using System;

// Token: 0x02000077 RID: 119
public class ReturnButton : ButtonScript
{
	// Token: 0x06000295 RID: 661 RVA: 0x000037C7 File Offset: 0x000019C7
	public override void ButtonConfirm()
	{
		ManagerScript.instance.PlayPausedSound(this.confirmClip);
		this.menuScript.MenuOpenClose(0, false);
	}

	// Token: 0x04000286 RID: 646
	public MenuScript menuScript;
}
