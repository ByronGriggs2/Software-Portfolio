using System;
using UnityEngine;

// Token: 0x02000078 RID: 120
public class VolumeButton : ButtonScript
{
	// Token: 0x06000297 RID: 663 RVA: 0x0000B29C File Offset: 0x0000949C
	public override void ButtonRefresh(string refType)
	{
		base.ButtonRefresh(refType);
		if (this.soundType == "musicVolume")
		{
			this.currentVolume = PlayerPrefs.GetFloat(this.soundType, 5f);
		}
		else
		{
			this.currentVolume = PlayerPrefs.GetFloat(this.soundType, 7f);
		}
		this.buttonText2.text = (this.currentVolume * 10f).ToString() + "%";
	}

	// Token: 0x06000298 RID: 664 RVA: 0x0000B31C File Offset: 0x0000951C
	public override void ButtonSwitch(bool additive)
	{
		if (additive)
		{
			this.currentVolume += 1f;
			if (this.currentVolume > 10f)
			{
				this.currentVolume = 0f;
			}
		}
		else
		{
			this.currentVolume -= 1f;
			if (this.currentVolume < 0f)
			{
				this.currentVolume = 10f;
			}
		}
		if (this.currentVolume != 0f)
		{
			ManagerScript.instance.PlayPausedSound(this.confirmClip);
		}
		this.currentVolume = Mathf.Round(this.currentVolume);
		PlayerPrefs.SetFloat(this.soundType, this.currentVolume);
		this.buttonText2.text = (this.currentVolume * 10f).ToString() + "%";
		ManagerScript.instance.VolumeChange(this.soundType, this.currentVolume / 10f);
	}

	// Token: 0x04000287 RID: 647
	public string soundType;

	// Token: 0x04000288 RID: 648
	private float currentVolume;
}
