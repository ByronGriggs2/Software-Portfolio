using System;

// Token: 0x02000010 RID: 16
public class CutterScript : ThrowableScript
{
	// Token: 0x06000048 RID: 72 RVA: 0x000022E0 File Offset: 0x000004E0
	public override void OnCollisionEnter2D()
	{
		this.Destruction();
	}
}
