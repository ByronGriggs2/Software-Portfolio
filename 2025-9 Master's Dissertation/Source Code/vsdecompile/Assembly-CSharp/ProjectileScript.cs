using System;
using UnityEngine;

// Token: 0x0200001D RID: 29
public abstract class ProjectileScript : MonoBehaviour
{
    // Token: 0x0600007A RID: 122 RVA: 0x00002519 File Offset: 0x00000719
    private void Awake()
	{
		this.bulletColl = base.GetComponent<Collider2D>();
		this.rb = base.GetComponent<Rigidbody2D>();
		this.animator = base.GetComponent<Animator>();
        
        this.PostAwake();
	}

	// Token: 0x0600007B RID: 123 RVA: 0x00002545 File Offset: 0x00000745
	public virtual void PostAwake()
	{
		this.PostAnim();
	}

	// Token: 0x0600007C RID: 124 RVA: 0x0000254D File Offset: 0x0000074D
	public virtual void PostAnim()
	{
		base.gameObject.SetActive(false);
		this.bulletColl.enabled = true;
	}

	// Token: 0x0600007D RID: 125 RVA: 0x000022E0 File Offset: 0x000004E0
	public virtual void OnCollisionEnter2D()
	{
		this.Destruction();
	}

	// Token: 0x0600007E RID: 126 RVA: 0x0000473C File Offset: 0x0000293C
	public virtual void Destruction()
	{
		if (this.trail != null)
		{
			this.trail.Clear();
		}
		if (base.gameObject.activeSelf)
		{
			this.animator.SetTrigger("hit");
			this.bulletColl.enabled = false;
			this.rb.velocity = Vector2.zero;
			base.Invoke("PostAnim", this.destructTime);
		}
	}

	// Token: 0x0400004A RID: 74
	public float speed = 8f;

	// Token: 0x0400004B RID: 75
	protected Rigidbody2D rb;

	// Token: 0x0400004C RID: 76
	public Animator animator;

	// Token: 0x0400004D RID: 77
	protected Collider2D bulletColl;

	// Token: 0x0400004E RID: 78
	public float destructTime = 0.3f;

	// Token: 0x0400004F RID: 79
	public TrailRenderer trail;
}
