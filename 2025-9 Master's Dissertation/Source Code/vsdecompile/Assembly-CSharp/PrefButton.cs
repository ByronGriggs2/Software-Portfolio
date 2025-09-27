using System;
using UnityEngine;

// Token: 0x02000074 RID: 116
public class PrefButton : ButtonScript
{
	// Token: 0x06000288 RID: 648 RVA: 0x0000B0C0 File Offset: 0x000092C0
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		if (refType == "regular")
		{
			if (this.pref == "fullscreen")
			{
				this.currentState = Screen.fullScreen;
			}
			else if (PlayerPrefs.GetInt(this.pref, 0) == 1)
			{
				this.currentState = true;
			}
			else
			{
				this.currentState = false;
			}
			if (this.currentState)
			{
				this.buttonText2.text = ManagerScript.instance.menuTxt[this.button2int + 1];
				return;
			}
			this.buttonText2.text = ManagerScript.instance.menuTxt[this.button2int];
		}
	}

	// Token: 0x06000289 RID: 649 RVA: 0x0000B168 File Offset: 0x00009368
	public override void ButtonConfirm()
	{
		ManagerScript.instance.PlayPausedSound(this.confirmClip);
		this.currentState = !this.currentState;
		if (this.pref == "fullscreen")
		{
			Screen.fullScreen = this.currentState;
			Cursor.visible = !this.currentState;
		}
		if (this.currentState)
		{
			this.buttonText2.text = ManagerScript.instance.menuTxt[this.button2int + 1];
			PlayerPrefs.SetInt(this.pref, 1);
			return;
		}
		this.buttonText2.text = ManagerScript.instance.menuTxt[this.button2int];
		PlayerPrefs.SetInt(this.pref, 0);
	}

	// Token: 0x0600028A RID: 650 RVA: 0x0000372C File Offset: 0x0000192C
	public override void ButtonSwitch(bool additive)
	{
		this.ButtonConfirm();
	}

	// Token: 0x04000282 RID: 642
	public string pref;

	// Token: 0x04000283 RID: 643
	private bool currentState;

	// Token: 0x04000284 RID: 644
	public int button2int;
}
