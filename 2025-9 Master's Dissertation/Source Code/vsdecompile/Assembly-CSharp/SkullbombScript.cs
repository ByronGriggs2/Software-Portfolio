using System;
using System.Collections;
using TMPro;
using UnityEngine;

// Token: 0x02000023 RID: 35
public class SkullbombScript : MonoBehaviour
{
    // Token: 0x06000099 RID: 153 RVA: 0x00004B30 File Offset: 0x00002D30
    public void Spawn()
	{
		base.transform.position = this.origin;
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("spawn");
		base.StartCoroutine(this.Arm());
		AwariaInterface.MainInterface.Instance.addHazard(this);
	}

	// Token: 0x0600009A RID: 154 RVA: 0x000026BB File Offset: 0x000008BB
	public IEnumerator Arm()
	{
		this.hittable = true;
		yield return new WaitForSeconds(0.7f);
		this.timer = base.StartCoroutine(this.TimerCount());
		this.armed = true;
		this.hittable = true;
		yield break;
	}

	// Token: 0x0600009B RID: 155 RVA: 0x00004B7C File Offset: 0x00002D7C
	private void FixedUpdate()
	{
		if (!this.armed)
		{
			return;
		}
		Vector2 a = this.player.transform.position;
		if (Vector2.Distance(a, base.transform.position) < 1.5f)
		{
			base.StartCoroutine(this.Explosion());
			return;
		}
		Vector2 normalized = (a - this.rb.position).normalized;
		this.rb.AddForce(normalized * 4f);
		if (this.rb.velocity.x > 0.1f)
		{
			this.spriteTransform.localScale = new Vector3(-1f, 1f, 1f);
			return;
		}
		if (this.rb.velocity.x < 0.1f)
		{
			this.spriteTransform.localScale = new Vector3(1f, 1f, 1f);
		}
	}

	// Token: 0x0600009C RID: 156 RVA: 0x000026CA File Offset: 0x000008CA
	public IEnumerator Explosion()
	{
		this.armed = false;
		base.StopCoroutine(this.timer);
		this.counter.text = "";
		this.animator.SetTrigger("explode");
		this.attackSource = ManagerScript.instance.PlayShortSound(this.attackClip[UnityEngine.Random.Range(0, this.attackClip.Length)], 0);
		yield return new WaitForSeconds(0.4f);
		this.hittable = false;
		if (Vector2.Distance(this.player.transform.position, base.transform.position) < 1f)
		{
			this.player.Gameover();
		}
		else
		{
			this.player.rotationScript.camScript.Shakedown(0.15f, 0.15f);
		}
		this.nova.Spread(base.transform.position);
		yield return new WaitForSeconds(1f);
		base.gameObject.SetActive(false);
		yield break;
	}

	// Token: 0x0600009D RID: 157 RVA: 0x00004C70 File Offset: 0x00002E70
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (collision.tag == "Player" && this.player.superDash == 2 && !this.player.inputsBlocked)
		{
			this.Dmg(true);
			return;
		}
		if (collision.tag == "veryDeadly")
		{
			this.hitOrigin = new Vector2(100f, 0f);
			this.Dmg(false);
		}
	}

	// Token: 0x0600009E RID: 158 RVA: 0x00004CE0 File Offset: 0x00002EE0
	public void Dmg(bool contact = true)
	{
		if (!contact)
		{
			this.nova.Wipe();
		}
		if (!this.hittable)
		{
			return;
		}
		this.hittable = false;
		this.armed = false;
		base.StopAllCoroutines();
		this.animator.SetTrigger("pushed");
		ManagerScript.instance.PlayShortSound(this.deathClip, 0);
		if (this.attackSource != null)
		{
			UnityEngine.Object.Destroy(this.attackSource.gameObject);
		}
		this.counter.text = "";
		UnityEngine.Object.Instantiate<GameObject>(this.dmgVfx, base.transform.position, base.transform.rotation);
		if (contact)
		{
			this.rb.AddForce(this.player.dashDir * 500f);
			return;
		}
		if (this.hitOrigin.x == 100f)
		{
			this.hitOrigin = Vector2.zero;
			this.rb.AddForce(this.hitOrigin * 500f);
			return;
		}
		this.hitOrigin = this.player.transform.position;
		Vector2 normalized = (this.origin - this.rb.position).normalized;
		this.rb.AddForce(normalized * 500f);
	}

	// Token: 0x0600009F RID: 159 RVA: 0x000026D9 File Offset: 0x000008D9
	public void Death()
	{
		UnityEngine.Object.Instantiate<GameObject>(this.deathVfx, base.transform.position, base.transform.rotation);
		base.gameObject.SetActive(false);
	}

	// Token: 0x060000A0 RID: 160 RVA: 0x00002709 File Offset: 0x00000909
	public IEnumerator TimerCount()
	{
		int num;
		for (int timeLeft = this.secondsToDie; timeLeft >= 0; timeLeft = num - 1)
		{
			this.counter.text = timeLeft.ToString();
			yield return new WaitForSeconds(1f);
			num = timeLeft;
		}
		if (this.armed)
		{
			base.StartCoroutine(this.Explosion());
		}
		yield break;
	}

	// Token: 0x060000A1 RID: 161 RVA: 0x00002718 File Offset: 0x00000918
	public void Wipe()
	{
		base.StopAllCoroutines();
		UnityEngine.Object.Destroy(this.nova.gameObject);
		base.gameObject.SetActive(false);
	}

	// Token: 0x0400006B RID: 107
	private bool armed;

	// Token: 0x0400006C RID: 108
	private bool hittable;

	// Token: 0x0400006D RID: 109
	public int secondsToDie = 9;

	// Token: 0x0400006E RID: 110
	public GameObject deathVfx;

	// Token: 0x0400006F RID: 111
	public GameObject dmgVfx;

	// Token: 0x04000070 RID: 112
	public Vector2 origin;

	// Token: 0x04000071 RID: 113
	public Animator animator;

	// Token: 0x04000072 RID: 114
	public BulletSpreadScript nova;

	// Token: 0x04000073 RID: 115
	public PlayerScript player;

	// Token: 0x04000074 RID: 116
	public Rigidbody2D rb;

	// Token: 0x04000075 RID: 117
	public Transform spriteTransform;

	// Token: 0x04000076 RID: 118
	public Coroutine timer;

	// Token: 0x04000077 RID: 119
	public TMP_Text counter;

	// Token: 0x04000078 RID: 120
	public AudioClip deathClip;

	// Token: 0x04000079 RID: 121
	public AudioClip[] attackClip;

	// Token: 0x0400007A RID: 122
	public AudioSource attackSource;

	// Token: 0x0400007B RID: 123
	private Vector2 hitOrigin;
}
