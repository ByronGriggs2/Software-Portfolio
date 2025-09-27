using System;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

// Token: 0x02000071 RID: 113
public class GalleryButton : ButtonScript
{
	// Token: 0x0600027B RID: 635 RVA: 0x000020D2 File Offset: 0x000002D2
	public override void ButtonHighlight(bool highlight, bool moveOut = false)
	{
	}

	// Token: 0x0600027C RID: 636 RVA: 0x0000368A File Offset: 0x0000188A
	public override void ButtonRefresh(string refType)
	{
		this.currentPage = 0;
		this.PageRefresh();
	}

	// Token: 0x0600027D RID: 637 RVA: 0x0000ACEC File Offset: 0x00008EEC
	public override void ButtonSwitch(bool additive)
	{
		ManagerScript.instance.PlayPausedSound(this.confirmClip);
		if (additive)
		{
			this.currentPage++;
			if (this.currentPage == this.ImageArray.Length)
			{
				this.currentPage = 0;
			}
		}
		else
		{
			this.currentPage--;
			if (this.currentPage < 0)
			{
				this.currentPage = this.ImageArray.Length - 1;
			}
		}
		this.PageRefresh();
	}

	// Token: 0x0600027E RID: 638 RVA: 0x0000AD60 File Offset: 0x00008F60
	public void PageRefresh()
	{
		if (PlayerPrefs.GetInt("beel", 0) != 1 && PlayerPrefs.GetInt("hardCount", 0) < this.requirementArray[this.currentPage])
		{
			this.requirementText1.text = "★ " + PlayerPrefs.GetInt("hardCount", 0).ToString() + " / " + this.requirementArray[this.currentPage].ToString();
			this.requirementText2.text = ManagerScript.instance.menuTxt[53];
			this.buttonText2.text = "";
			this.curtainObject.SetActive(true);
		}
		else
		{
			this.buttonText2.text = this.menuScript.galleryTxt[this.currentPage];
			this.curtainObject.SetActive(false);
			if (this.currentPage < 14)
			{
				this.buttonText2.color = Color.green;
			}
			else
			{
				this.buttonText2.color = Color.white;
			}
		}
		this.buttonText.text = (this.currentPage + 1).ToString() + " / " + this.ImageArray.Length.ToString();
		this.galleryImage.sprite = this.ImageArray[this.currentPage];
		this.rectTransform.sizeDelta = new Vector2((float)this.galleryImage.sprite.texture.width, (float)this.galleryImage.sprite.texture.height);
	}

	// Token: 0x0400026F RID: 623
	public MenuScript menuScript;

	// Token: 0x04000270 RID: 624
	public Sprite[] ImageArray;

	// Token: 0x04000271 RID: 625
	public int[] requirementArray;

	// Token: 0x04000272 RID: 626
	public Image galleryImage;

	// Token: 0x04000273 RID: 627
	public RectTransform rectTransform;

	// Token: 0x04000274 RID: 628
	private int currentPage;

	// Token: 0x04000275 RID: 629
	public TMP_Text requirementText1;

	// Token: 0x04000276 RID: 630
	public TMP_Text requirementText2;

	// Token: 0x04000277 RID: 631
	public GameObject curtainObject;
}
