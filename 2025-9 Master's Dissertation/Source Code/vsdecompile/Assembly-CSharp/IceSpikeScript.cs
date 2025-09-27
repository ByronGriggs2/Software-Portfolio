using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000014 RID: 20
public class IceSpikeScript : MonoBehaviour
{
    // Token: 0x06000050 RID: 80 RVA: 0x00004380 File Offset: 0x00002580
    public void Spawn(Vector2 target, Sprite randomSprite)
	{
		base.transform.position = new Vector3(target.x + UnityEngine.Random.Range(-0.1f, 0.1f), target.y + UnityEngine.Random.Range(-0.1f, 0.1f));
		this.spikeSprite.sprite = randomSprite;
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("spawn");
		this.lifespanRoutine = base.StartCoroutine(this.Lifespan());
        AwariaInterface.MainInterface.Instance.addHazard(this);
    }

	// Token: 0x06000051 RID: 81 RVA: 0x0000232F File Offset: 0x0000052F
	private IEnumerator Lifespan()
	{
		yield return new WaitForSeconds(0.2f);
		this.hitbox.enabled = true;
		yield return new WaitForSeconds(4f);
		base.StartCoroutine(this.Crack());
		yield break;
	}

	// Token: 0x06000052 RID: 82 RVA: 0x0000233E File Offset: 0x0000053E
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (collision.tag == "Player")
		{
			collision.GetComponent<PlayerScript>().Gameover();
			base.StartCoroutine(this.Crack());
		}
	}

	// Token: 0x06000053 RID: 83 RVA: 0x0000236A File Offset: 0x0000056A
	private IEnumerator Crack()
	{
		if (this.hitbox.enabled)
		{
			this.hitbox.enabled = false;
			this.animator.SetTrigger("crack");
			yield return new WaitForSeconds(0.4f);
			base.StopCoroutine(this.lifespanRoutine);
			base.gameObject.SetActive(false);
		}
		yield break;
	}

	// Token: 0x0400002E RID: 46
	public CircleCollider2D hitbox;

	// Token: 0x0400002F RID: 47
	public SpriteRenderer spikeSprite;

	// Token: 0x04000030 RID: 48
	public Animator animator;

	// Token: 0x04000031 RID: 49
	private Coroutine lifespanRoutine;
}
