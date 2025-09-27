using System;
using System.IO;
using UnityEngine;

// Token: 0x02000073 RID: 115
public class NextMenuButton : ButtonScript
{
	// Token: 0x06000284 RID: 644 RVA: 0x0000AFB8 File Offset: 0x000091B8
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		if (this.nextMenu == 5 && this.menuScript.galleryUnlocked)
		{
			this.buttonText.text = ManagerScript.instance.menuTxt[this.textInt + 3] + " ★";
		}
	}

	// Token: 0x06000285 RID: 645 RVA: 0x000036FA File Offset: 0x000018FA
	public override void ButtonHighlight(bool highlight, bool moveOut = false)
	{
		base.ButtonHighlight(highlight, moveOut);
		if (this.menuScript.currentMenu == 1 && highlight)
		{
			this.menuScript.animator.SetTrigger(this.aniTrigger);
		}
	}

	// Token: 0x06000286 RID: 646 RVA: 0x0000B00C File Offset: 0x0000920C
	public override void ButtonConfirm()
	{
		ManagerScript.instance.PlayLongSound(this.confirmClip);
		if (this.nextMenu == 5)
		{
			if (this.menuScript.galleryTxt.Length == 0)
			{
				this.menuScript.galleryTxt = File.ReadAllLines(ManagerScript.instance.directory + "/local/g.json");
			}
			this.menuScript.MenuSwitch(this.nextMenu);
			return;
		}
		if (this.nextMenu == 666)
		{
			Application.Quit();
			return;
		}
		if (this.nextMenu == 0)
		{
			this.menuScript.MenuSwitch(this.menuScript.submenuArray[2].parentMenu);
			return;
		}
		this.menuScript.MenuSwitch(this.nextMenu);
	}

	// Token: 0x0400027F RID: 639
	public MenuScript menuScript;

	// Token: 0x04000280 RID: 640
	public int nextMenu;

	// Token: 0x04000281 RID: 641
	public string aniTrigger;
}
