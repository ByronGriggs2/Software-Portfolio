using System;
using UnityEngine;

// Token: 0x02000075 RID: 117
public class ProgressButton : ButtonScript
{
	// Token: 0x0600028C RID: 652 RVA: 0x00003734 File Offset: 0x00001934
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		this.currentProgress = PlayerPrefs.GetInt("lvl", 1);
		this.buttonText2.text = ManagerScript.instance.chapTxt[this.currentProgress - 1];
	}

	// Token: 0x0600028D RID: 653 RVA: 0x0000B21C File Offset: 0x0000941C
	public override void ButtonSwitch(bool additive)
	{
		if (additive)
		{
			this.currentProgress++;
			if (this.currentProgress == 13)
			{
				this.currentProgress = 1;
			}
		}
		else
		{
			this.currentProgress--;
			if (this.currentProgress < 1)
			{
				this.currentProgress = 12;
			}
		}
		PlayerPrefs.SetInt("lvl", this.currentProgress);
		this.buttonText2.text = ManagerScript.instance.chapTxt[this.currentProgress - 1];
	}

	// Token: 0x04000285 RID: 645
	private int currentProgress;
}
