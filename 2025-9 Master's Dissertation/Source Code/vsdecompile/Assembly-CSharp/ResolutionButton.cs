using System;

// Token: 0x02000076 RID: 118
public class ResolutionButton : ButtonScript
{
	// Token: 0x0600028F RID: 655 RVA: 0x000020D2 File Offset: 0x000002D2
	private void Start()
	{
	}

	// Token: 0x06000290 RID: 656 RVA: 0x0000376C File Offset: 0x0000196C
	public override void ButtonHighlight(bool highlight, bool moveOut = false)
	{
		base.ButtonHighlight(highlight, moveOut);
		if (!highlight && moveOut)
		{
			ManagerScript.instance.ResolutionSet();
		}
	}

	// Token: 0x06000291 RID: 657 RVA: 0x00003788 File Offset: 0x00001988
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		ManagerScript.instance.ResolutionPrinter(this, true);
	}

	// Token: 0x06000292 RID: 658 RVA: 0x0000379D File Offset: 0x0000199D
	public override void ButtonConfirm()
	{
		ManagerScript.instance.ResolutionSet();
	}

	// Token: 0x06000293 RID: 659 RVA: 0x000037A9 File Offset: 0x000019A9
	public override void ButtonSwitch(bool additive)
	{
		ManagerScript.instance.PlayPausedSound(this.confirmClip);
		ManagerScript.instance.ResolutionSwitch(this, additive);
	}
}
