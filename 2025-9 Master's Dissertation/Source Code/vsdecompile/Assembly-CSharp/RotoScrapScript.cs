using System;
using UnityEngine;

// Token: 0x0200001E RID: 30
public class RotoScrapScript : MonoBehaviour
{
	// Token: 0x06000080 RID: 128 RVA: 0x000047AC File Offset: 0x000029AC
	public void ReCast(Vector2 target)
	{
		this.scrapSprite.sprite = this.spritePool[UnityEngine.Random.Range(0, this.spritePool.Length)];
		this.center = (base.transform.position = target);
		this.tempAngle = this.angle;
		this.tempSpeed = this.speed;
		this.tempRadius = this.radius;
		base.gameObject.SetActive(true);
		this.anim.SetTrigger("recast");
	}

	// Token: 0x06000081 RID: 129 RVA: 0x00004838 File Offset: 0x00002A38
	private void Update()
	{
		base.transform.position = new Vector3(this.center.x + Mathf.Cos(this.tempAngle) * this.tempRadius, this.center.y + Mathf.Sin(this.tempAngle) * this.tempRadius, 0f);
		this.tempSpeed += 0.5f;
		this.tempAngle += Time.deltaTime * this.tempSpeed;
		this.tempRadius -= Time.deltaTime * 0.5f;
	}

	// Token: 0x04000050 RID: 80
	public float speed = 2f;

	// Token: 0x04000051 RID: 81
	public float radius = 1f;

	// Token: 0x04000052 RID: 82
	public float angle;

	// Token: 0x04000053 RID: 83
	public Vector2 center;

	// Token: 0x04000054 RID: 84
	public Animator anim;

	// Token: 0x04000055 RID: 85
	public Sprite[] spritePool;

	// Token: 0x04000056 RID: 86
	public SpriteRenderer scrapSprite;

	// Token: 0x04000057 RID: 87
	private float tempAngle;

	// Token: 0x04000058 RID: 88
	private float tempSpeed;

	// Token: 0x04000059 RID: 89
	private float tempRadius;
}
