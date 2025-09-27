using System;
using TMPro;
using UnityEngine;

// Token: 0x02000034 RID: 52
public class EndCreditsScript : MonoBehaviour
{
	private AwariaInterface.IInputProvider byronInputs;
	private void Start()
	{
        this.byronInputs = GameObject.FindObjectOfType<AwariaInterface.MainInterface>() as AwariaInterface.IInputProvider;
    }

	// Token: 0x060000EB RID: 235 RVA: 0x000029A6 File Offset: 0x00000BA6
	private void Update()
	{
		if (byronInputs.getCancel())
		{
			this.BackToMenu();
		}
	}

	// Token: 0x060000EC RID: 236 RVA: 0x00005880 File Offset: 0x00003A80
	private void CreditsRefresh()
	{
		if (this.textIndex == 33)
		{
			this.BackToMenu();
		}
		this.roleText.text = ManagerScript.instance.menuTxt[this.textIndex];
		this.nameText.text = ManagerScript.instance.menuTxt[this.textIndex + 1];
		this.textIndex += 2;
		if (this.nameText.text == "X")
		{
			this.CreditsRefresh();
		}
	}

	// Token: 0x060000ED RID: 237 RVA: 0x000029BA File Offset: 0x00000BBA
	private void BackToMenu()
	{
		if (this.inputBlock)
		{
			return;
		}
		this.inputBlock = true;
		ManagerScript.instance.mainMenu = true;
		this.tranScript.Transition();
	}

	// Token: 0x040000BB RID: 187
	public TransitionScript tranScript;

	// Token: 0x040000BC RID: 188
	public TMP_Text roleText;

	// Token: 0x040000BD RID: 189
	public TMP_Text nameText;

	// Token: 0x040000BE RID: 190
	private int textIndex = 23;

	// Token: 0x040000BF RID: 191
	private bool inputBlock;
}
