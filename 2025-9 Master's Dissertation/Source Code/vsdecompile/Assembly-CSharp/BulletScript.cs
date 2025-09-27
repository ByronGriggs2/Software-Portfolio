using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000008 RID: 8
public class BulletScript : ProjectileScript
{
    // Token: 0x0600001E RID: 30 RVA: 0x000021BC File Offset: 0x000003BC
    public override void PostAwake()
	{
		this.baseOrigin = base.transform.localPosition;
	}

	// Token: 0x0600001F RID: 31 RVA: 0x00003DD8 File Offset: 0x00001FD8
	public void Cast()
	{
		if (this.bulletSprite != null)
		{
			this.bulletSprite.sprite = this.spritePool[UnityEngine.Random.Range(0, this.spritePool.Length)];
		}
		if (this.deathVfx != null)
		{
			this.bulletColl.enabled = false;
			base.gameObject.SetActive(true);
			this.animator.SetTrigger("fire");
			base.StartCoroutine(this.Spawn());
			return;
		}
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("fire");
		if (this.trail != null)
		{
			this.trail.Clear();
		}
		this.rb.AddForce(this.bulletDir.normalized * this.speed, ForceMode2D.Impulse);
		// Safety net, should always evaluate true
		if (this.gameObject.tag == "Deadly")
			AwariaInterface.MainInterface.Instance.addHazard(this);
		else
			Debug.Log("Not deadly");
	}

	// Token: 0x06000020 RID: 32 RVA: 0x000021CF File Offset: 0x000003CF
	public IEnumerator Spawn()
	{
		base.gameObject.tag = "Untagged";
		this.bulletColl.enabled = true;
		yield return new WaitForSeconds(1f);
		this.bulletColl.enabled = false;
		base.gameObject.tag = "Deadly";
		this.bulletColl.enabled = true;
		this.animator.SetTrigger("idle");
		this.rb.AddForce(this.bulletDir.normalized * this.speed, ForceMode2D.Impulse);
        if (this.gameObject.tag == "Deadly")
            AwariaInterface.MainInterface.Instance.addHazard(this);
        yield break;
	}

	// Token: 0x06000021 RID: 33 RVA: 0x000021DE File Offset: 0x000003DE
	public override void PostAnim()
	{
		base.PostAnim();
		base.transform.localPosition = this.baseOrigin;
	}

	// Token: 0x06000022 RID: 34 RVA: 0x00003EB0 File Offset: 0x000020B0
	public override void Destruction()
	{
		if (this.deathVfx != null && base.gameObject.activeSelf)
		{
			UnityEngine.Object.Instantiate<GameObject>(this.deathVfx, base.transform.position, base.transform.rotation);
			if (this.audible)
			{
				ManagerScript.instance.PlayShortCommonSound(this.deathrattle[UnityEngine.Random.Range(0, this.deathrattle.Length)]);
			}
		}
		base.Destruction();
	}

	private void OnDestroy()
	{
	}

	// Token: 0x06000023 RID: 35 RVA: 0x00003F28 File Offset: 0x00002128
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (this.deathVfx != null && collision.tag == "Player")
		{
			PlayerScript component = collision.GetComponent<PlayerScript>();
			if (component.superDash == 2 && !component.inputsBlocked)
			{
				base.StopAllCoroutines();
				this.Destruction();
			}
		}
	}

	// Token: 0x06000024 RID: 36 RVA: 0x000021F7 File Offset: 0x000003F7
	private IEnumerator Killed(PlayerScript player)
	{
		player.inputsBlocked = true;
		Time.timeScale = 0f;
		yield return new WaitForSecondsRealtime(0.05f);
		Time.timeScale = 1f;
		player.inputsBlocked = false;
		this.Destruction();
		yield break;
	}

	// Token: 0x04000011 RID: 17
	public Vector2 bulletDir;

	// Token: 0x04000012 RID: 18
	private Vector3 baseOrigin;

	// Token: 0x04000013 RID: 19
	public Sprite[] spritePool;

	// Token: 0x04000014 RID: 20
	public SpriteRenderer bulletSprite;

	// Token: 0x04000015 RID: 21
	public GameObject deathVfx;

	// Token: 0x04000016 RID: 22
	public AudioClip[] deathrattle;

	// Token: 0x04000017 RID: 23
	public bool audible;
}
