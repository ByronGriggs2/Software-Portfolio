using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000029 RID: 41
public class TrapScript : MonoBehaviour
{
	// Token: 0x060000B9 RID: 185 RVA: 0x000027C5 File Offset: 0x000009C5
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (collision.tag == "Player" && this.set)
		{
			base.StartCoroutine(this.Activation(collision.transform));
		}
	}

	// Token: 0x060000BA RID: 186 RVA: 0x000027F4 File Offset: 0x000009F4
	private void Start()
	{
		base.StartCoroutine(this.Setup());
        AwariaInterface.MainInterface.Instance.addHazard(this);
    }

	// Token: 0x060000BB RID: 187 RVA: 0x00002803 File Offset: 0x00000A03
	private IEnumerator Setup()
	{
		yield return new WaitForSeconds(0.1f);
		this.trapSprite.sprite = this.randomSprite;
		yield return new WaitForSeconds(0.3f);
		this.set = true;
		this.trapCollider.enabled = true;
		yield break;
	}

	// Token: 0x060000BC RID: 188 RVA: 0x00002812 File Offset: 0x00000A12
	private IEnumerator Activation(Transform playerTransform)
	{
		this.set = false;
		this.animator.SetTrigger("activate");
		ManagerScript.instance.PlayShortSound(this.soundArray[UnityEngine.Random.Range(0, this.soundArray.Length)], 0);
		yield return new WaitForSeconds(0.3f);
		if (Vector3.Distance(playerTransform.position, base.transform.position) < 1f)
		{
			this.rotationScript.Gameover(playerTransform, true, 1);
		}
		yield return new WaitForSeconds(0.8f);
		UnityEngine.Object.Destroy(base.gameObject);
		yield break;
	}

	// Token: 0x04000089 RID: 137
	public Animator animator;

	// Token: 0x0400008A RID: 138
	public RotationScript rotationScript;

	// Token: 0x0400008B RID: 139
	public SpriteRenderer trapSprite;

	// Token: 0x0400008C RID: 140
	public Sprite randomSprite;

	// Token: 0x0400008D RID: 141
	public CircleCollider2D trapCollider;

	// Token: 0x0400008E RID: 142
	public AudioClip[] soundArray;

	// Token: 0x0400008F RID: 143
	private bool set;
}
