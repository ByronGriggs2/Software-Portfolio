using System;
using TMPro;
using UnityEngine;

// Token: 0x0200006E RID: 110
public abstract class ButtonScript : MonoBehaviour
{
	// Token: 0x06000270 RID: 624 RVA: 0x0000AB9C File Offset: 0x00008D9C
	public virtual void ButtonHighlight(bool highlight, bool moveOut = false)
	{
		this.finger.SetActive(highlight);
		if (highlight)
		{
			this.buttonText.color = Color.white;
			if (this.buttonText2 != null)
			{
				this.buttonText2.color = Color.white;
				return;
			}
		}
		else
		{
			this.buttonText.color = this.nonWhite;
			if (this.buttonText2 != null)
			{
				this.buttonText2.color = this.nonWhite;
			}
		}
	}

	// Token: 0x06000271 RID: 625 RVA: 0x000035CD File Offset: 0x000017CD
	public virtual void ButtonRefresh(string refType)
	{
		if (refType == "regular")
		{
			this.buttonText.text = ManagerScript.instance.menuTxt[this.textInt];
		}
	}

	// Token: 0x06000272 RID: 626 RVA: 0x000020D2 File Offset: 0x000002D2
	public virtual void ButtonConfirm()
	{
	}

	// Token: 0x06000273 RID: 627 RVA: 0x000020D2 File Offset: 0x000002D2
	public virtual void ButtonSwitch(bool additive)
	{
	}

	// Token: 0x04000262 RID: 610
	public GameObject finger;

	// Token: 0x04000263 RID: 611
	public TMP_Text buttonText;

	// Token: 0x04000264 RID: 612
	public TMP_Text buttonText2;

	// Token: 0x04000265 RID: 613
	public int textInt;

	// Token: 0x04000266 RID: 614
	public Color nonWhite = Color.green;

	// Token: 0x04000267 RID: 615
	public AudioClip confirmClip;
}
