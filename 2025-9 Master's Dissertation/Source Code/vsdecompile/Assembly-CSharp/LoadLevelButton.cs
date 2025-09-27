using System;
using TMPro;
using UnityEngine;

// Token: 0x02000072 RID: 114
public class LoadLevelButton : ButtonScript
{
	// Token: 0x06000280 RID: 640 RVA: 0x00003699 File Offset: 0x00001899
	public override void ButtonHighlight(bool highlight, bool moveOut = false)
	{
		base.ButtonHighlight(highlight, moveOut);
		if (this.menuScript.currentMenu != 0 && highlight)
		{
			this.skullAnimator.SetTrigger(this.difficulty);
		}
	}

	// Token: 0x06000281 RID: 641 RVA: 0x000036C6 File Offset: 0x000018C6
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		if (this.menuScript.currentMenu != 0)
		{
			this.buttonText2.text = ManagerScript.instance.menuTxt[this.textInt + 1];
		}
	}

	// Token: 0x06000282 RID: 642 RVA: 0x0000AEF0 File Offset: 0x000090F0
	public override void ButtonConfirm()
	{
		// Byron mod. Clean up the cached generator data when deliberately leaving a level.
		if (this.textInt == 15)
		{
			AwariaInterface.MainInterface.Instance.levelEndCleanup();
        }

		ManagerScript.instance.PlayLongSound(this.confirmClip);
		if (this.menuScript.currentMenu == 0)
		{
			ManagerScript.instance.mainMenu = true;
			this.menuScript.NextLevel(0f);
			return;
		}
		PlayerPrefs.SetInt("mode", int.Parse(this.difficulty));
		int nextLevel = ManagerScript.instance.nextLevel;
		int num;
		if (nextLevel < 10)
		{
			num = 0;
		}
		else if (nextLevel == 13)
		{
			num = 2;
		}
		else
		{
			num = 1;
		}
		this.introAnimator.SetTrigger("start");
		this.introAnimator2.SetInteger("scene", num);
		this.textbox.text = ManagerScript.instance.menuTxt[61 + num];
		this.menuScript.NextLevel(1.7f);
	}

	// Token: 0x04000278 RID: 632
	public SubmenuScript chapterScript;

	// Token: 0x04000279 RID: 633
	public MenuScript menuScript;

	// Token: 0x0400027A RID: 634
	public Animator skullAnimator;

	// Token: 0x0400027B RID: 635
	public Animator introAnimator;

	// Token: 0x0400027C RID: 636
	public Animator introAnimator2;

	// Token: 0x0400027D RID: 637
	public TMP_Text textbox;

	// Token: 0x0400027E RID: 638
	public string difficulty;
}
