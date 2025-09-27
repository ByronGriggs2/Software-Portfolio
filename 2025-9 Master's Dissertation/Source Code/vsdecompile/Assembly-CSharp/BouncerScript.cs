using System;
using UnityEngine;

// Token: 0x02000007 RID: 7
public class BouncerScript : MonoBehaviour
{
	// Token: 0x06000019 RID: 25 RVA: 0x00002135 File Offset: 0x00000335
	private void Awake()
	{
		this.rb = base.GetComponent<Rigidbody2D>();
		this.PostAnim();
	}

	// Token: 0x0600001A RID: 26 RVA: 0x00002149 File Offset: 0x00000349
	public void Cast(Vector3 origin)
	{
		base.transform.position = origin;
		base.gameObject.SetActive(true);
		this.rb.AddForce(this.direction * 0.5f, ForceMode2D.Impulse);
	}

	// Token: 0x0600001B RID: 27 RVA: 0x0000217F File Offset: 0x0000037F
	public void PostAnim()
	{
		base.gameObject.SetActive(false);
	}

	// Token: 0x0600001C RID: 28 RVA: 0x0000218D File Offset: 0x0000038D
	public void Destruction()
	{
		if (base.gameObject.activeSelf)
		{
			this.rb.velocity = Vector2.zero;
			base.Invoke("PostAnim", 0.3f);
		}
	}

	// Token: 0x0400000D RID: 13
	public Vector2 direction;

	// Token: 0x0400000E RID: 14
	private Animator animator;

	// Token: 0x0400000F RID: 15
	private Rigidbody2D rb;

	// Token: 0x04000010 RID: 16
	private Vector2 vel;
}
