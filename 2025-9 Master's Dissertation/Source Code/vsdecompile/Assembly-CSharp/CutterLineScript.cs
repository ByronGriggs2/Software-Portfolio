using System;
using UnityEngine;

// Token: 0x0200000F RID: 15
public class CutterLineScript : MonoBehaviour
{
	// Token: 0x06000044 RID: 68 RVA: 0x000022C0 File Offset: 0x000004C0
	private void Awake()
	{
		this.animator = base.GetComponent<Animator>();
		this.lineRender = base.GetComponent<LineRenderer>();
		this.PostAnim();
	}

	// Token: 0x06000045 RID: 69 RVA: 0x00004290 File Offset: 0x00002490
	public void DisplayLine(Vector3 origin, Vector2 dir)
	{
		RaycastHit2D raycastHit2D = Physics2D.Raycast(origin, dir, float.PositiveInfinity, this.layerMask);
		this.lineRender.SetPosition(0, origin);
		this.lineRender.SetPosition(1, raycastHit2D.point);
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("display");
		base.Invoke("PostAnim", 0.4f);
	}

	// Token: 0x06000046 RID: 70 RVA: 0x0000217F File Offset: 0x0000037F
	public void PostAnim()
	{
		base.gameObject.SetActive(false);
	}

	// Token: 0x04000028 RID: 40
	public LayerMask layerMask;

	// Token: 0x04000029 RID: 41
	private Animator animator;

	// Token: 0x0400002A RID: 42
	private LineRenderer lineRender;
}
