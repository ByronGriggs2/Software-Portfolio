using System;
using TMPro;
using UnityEngine;

// Token: 0x0200007C RID: 124
public class SubmenuScript : MonoBehaviour
{
	// Token: 0x060002B3 RID: 691 RVA: 0x0000BEAC File Offset: 0x0000A0AC
	public void MenuRefresh()
	{
		if (this.chapterSelect)
		{
			this.bossText.text = ManagerScript.instance.chapTxt[13];
			this.hardText.text = "★ " + ManagerScript.instance.chapTxt[27];
			this.maxIndex = PlayerPrefs.GetInt("lvl", 1);
			this.refreshType = "chapter";
		}
		else if (this.refreshType == "regular")
		{
			this.maxIndex = this.buttonArray.Length;
		}
		for (int i = 0; i < this.buttonArray.Length; i++)
		{
			if (this.chapterSelect && this.maxIndex == i)
			{
				this.refreshType = "";
			}
			this.buttonArray[i].ButtonRefresh(this.refreshType);
		}
		this.refreshType = "simple";
	}

	// Token: 0x060002B4 RID: 692 RVA: 0x0000BF88 File Offset: 0x0000A188
	public void SpriteRefresh(string trigger, int chapterIndex)
	{
		this.frameAnimator.SetTrigger("switch");
		this.ggAnimator.SetTrigger(trigger);
		this.talkText.text = ManagerScript.instance.chapTxt[chapterIndex + 14];
		this.chapterText2.text = (this.chapterText.text = ManagerScript.instance.chapTxt[chapterIndex]);
		if (PlayerPrefs.GetInt((chapterIndex + 1).ToString() + "H", 0) >= 1)
		{
			this.hardText.enabled = true;
			return;
		}
		this.hardText.enabled = false;
	}

	// Token: 0x040002A8 RID: 680
	public ButtonScript[] buttonArray;

	// Token: 0x040002A9 RID: 681
	public GameObject submenuCore;

	// Token: 0x040002AA RID: 682
	public string refreshType = "regular";

	// Token: 0x040002AB RID: 683
	public int maxIndex;

	// Token: 0x040002AC RID: 684
	public bool bar;

	// Token: 0x040002AD RID: 685
	public Color barColor;

	// Token: 0x040002AE RID: 686
	public int barName;

	// Token: 0x040002AF RID: 687
	public string aniTrigger;

	// Token: 0x040002B0 RID: 688
	public int parentMenu;

	// Token: 0x040002B1 RID: 689
	public bool unusualNavigation;

	// Token: 0x040002B2 RID: 690
	public bool chapterSelect;

	// Token: 0x040002B3 RID: 691
	public TMP_Text talkText;

	// Token: 0x040002B4 RID: 692
	public TMP_Text chapterText;

	// Token: 0x040002B5 RID: 693
	public TMP_Text chapterText2;

	// Token: 0x040002B6 RID: 694
	public Animator ggAnimator;

	// Token: 0x040002B7 RID: 695
	public Animator frameAnimator;

	// Token: 0x040002B8 RID: 696
	public TMP_Text hardText;

	// Token: 0x040002B9 RID: 697
	public TMP_Text bossText;

	// Token: 0x040002BA RID: 698
	public TMP_Text[] deathCount;

	// Token: 0x040002BB RID: 699
	public bool inputMenu;

	// Token: 0x040002BC RID: 700
	public GameObject firstInput;

	// Token: 0x040002BD RID: 701
	public string controlName;
}
