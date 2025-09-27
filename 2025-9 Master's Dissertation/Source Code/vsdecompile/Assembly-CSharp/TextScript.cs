using System;
using TMPro;
using UnityEngine;

// Token: 0x0200007D RID: 125
public class TextScript : MonoBehaviour
{
	// Token: 0x060002B6 RID: 694 RVA: 0x00003865 File Offset: 0x00001A65
	private void Start()
	{
		this.txt = base.GetComponent<TMP_Text>();
		this.txt.text = ManagerScript.instance.menuTxt[this.txtInt];
	}

	// Token: 0x040002BE RID: 702
	private TMP_Text txt;

	// Token: 0x040002BF RID: 703
	public int txtInt;
}
