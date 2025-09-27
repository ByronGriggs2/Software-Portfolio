using System;

// Token: 0x02000070 RID: 112
public class CreditButton : ButtonScript
{
	// Token: 0x06000279 RID: 633 RVA: 0x0000364B File Offset: 0x0000184B
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		if (refType == "regular" && ManagerScript.instance.menuTxt[30] == "X")
		{
			this.buttonText.text = "";
		}
	}
}
