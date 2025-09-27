using AwariaInterface;
using System;
using UnityEngine;

// Token: 0x0200006F RID: 111
public class ChapterButton : ButtonScript
{
	// Token: 0x06000275 RID: 629 RVA: 0x0000AC18 File Offset: 0x00008E18
	public override void ButtonHighlight(bool highlight, bool moveOut = false)
	{
		base.ButtonHighlight(highlight, moveOut);
		if (highlight)
		{
			this.chapterScript.SpriteRefresh(this.ggTrigger, int.Parse(this.chapterSymbol) - 1);
			this.mapAnimator.SetTrigger("on");
			return;
		}
		this.mapAnimator.SetTrigger("off");
	}

	// Token: 0x06000276 RID: 630 RVA: 0x0000AC70 File Offset: 0x00008E70
	public override void ButtonRefresh(string refType)
	{
		if (!(refType == "chapter"))
		{
			if (refType != "simple")
			{
				this.buttonText.text = refType;
			}
			return;
		}
		if (PlayerPrefs.GetInt(this.chapterSymbol + "H", 0) >= 1)
		{
			this.buttonText.text = this.chapterSymbol + "★";
			return;
		}
		this.buttonText.text = this.chapterSymbol;
	}

	// Token: 0x06000277 RID: 631 RVA: 0x0000360B File Offset: 0x0000180B
	public override void ButtonConfirm()
	{
		ManagerScript.instance.PlayPausedSound(this.confirmClip);
		ManagerScript.instance.nextLevel = int.Parse(this.chapterSymbol);
		this.menuScript.MenuSwitch(this.nextMenu);
		MainInterface.Instance.setCurrentLevel(int.Parse(this.chapterSymbol));
    }

	// Token: 0x04000268 RID: 616
	public SubmenuScript chapterScript;

	// Token: 0x04000269 RID: 617
	public MenuScript menuScript;

	// Token: 0x0400026A RID: 618
	public AudioClip chapterTrack;

	// Token: 0x0400026B RID: 619
	public string chapterSymbol;

	// Token: 0x0400026C RID: 620
	public string ggTrigger;

	// Token: 0x0400026D RID: 621
	public Animator mapAnimator;

	// Token: 0x0400026E RID: 622
	public int nextMenu;
}
