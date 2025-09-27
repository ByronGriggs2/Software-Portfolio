using System;
using System.IO;
using Steamworks;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

// Token: 0x02000031 RID: 49
public class CutsceneScript : MonoBehaviour
{
	// Token: 0x060000E2 RID: 226 RVA: 0x000028F4 File Offset: 0x00000AF4
	private void Start()
	{
		this.txt = File.ReadAllLines(ManagerScript.instance.directory + this.txtDir);
		this.animator = base.GetComponent<Animator>();
	}

	// Token: 0x060000E3 RID: 227 RVA: 0x0000560C File Offset: 0x0000380C
	public void InputConfirm()
	{
		if (!this.sceneReady)
		{
			return;
		}
		this.sceneReady = false;
		if (this.currentScene == this.sceneArray.Length)
		{
			ManagerScript.instance.LevelComplete();
			this.tranScript.Transition();
		}
		else
		{
			if (this.sceneArray[this.currentScene].type == "prekiss")
			{
				ManagerScript.instance.PlayShortSound(this.kissingClip[0], 1);
			}
			else
			{
				if (this.textAnimator.isActiveAndEnabled)
				{
					this.textAnimator.SetTrigger("appear");
				}
				this.nameText.text = this.txt[this.sceneArray[this.currentScene].nameLine];
				this.dialogueText.text = this.txt[this.sceneArray[this.currentScene].dialogueLine];
				this.scene.sprite = this.sceneArray[this.currentScene].sceneSprite;
				this.cutsceneAnim.SetActive(false);
				if (this.sceneArray[this.currentScene].type != "next")
				{
					this.kissText.text = ManagerScript.instance.menuTxt[int.Parse(this.sceneArray[this.currentScene].type)];
					this.kissText.enabled = true;
					if (this.sceneArray[this.currentScene].type == "17")
					{
						ManagerScript.instance.PlayShortSound(this.kissingClip[1], 1);
					}
					else
					{
						ManagerScript.instance.PlayShortSound(this.kissingClip[2], 1);
					}
				}
				else
				{
					ManagerScript.instance.PlayShortSound(this.swishClip, 1);
					this.kissText.enabled = false;
				}
			}
			this.animator.SetTrigger(this.sceneArray[this.currentScene].type);
			if (this.sceneArray[this.currentScene].achiev == "anim")
			{
				this.cutsceneAnim.SetActive(true);
			}
			else if (this.sceneArray[this.currentScene].achiev != "" && ManagerScript.instance.steam && SteamManager.Initialized)
			{
				SteamUserStats.SetAchievement(this.sceneArray[this.currentScene].achiev);
				SteamUserStats.StoreStats();
			}
		}
		this.currentScene++;
	}

	// Token: 0x060000E4 RID: 228 RVA: 0x00002922 File Offset: 0x00000B22
	private void CutsceneReady()
	{
		this.sceneReady = true;
	}

	// Token: 0x060000E5 RID: 229 RVA: 0x0000292B File Offset: 0x00000B2B
	private void CutsceneStart()
	{
		this.InputConfirm();
		this.playerScript.inputsBlocked = false;
		this.playerScript.inCutscene = true;
	}

	// Token: 0x040000A9 RID: 169
	public TMP_Text nameText;

	// Token: 0x040000AA RID: 170
	public TMP_Text dialogueText;

	// Token: 0x040000AB RID: 171
	public TMP_Text kissText;

	// Token: 0x040000AC RID: 172
	public Image scene;

	// Token: 0x040000AD RID: 173
	public string[] txt;

	// Token: 0x040000AE RID: 174
	public string txtDir;

	// Token: 0x040000AF RID: 175
	public PlayerScript playerScript;

	// Token: 0x040000B0 RID: 176
	public TransitionScript tranScript;

	// Token: 0x040000B1 RID: 177
	public Animator textAnimator;

	// Token: 0x040000B2 RID: 178
	public GameObject cutsceneAnim;

	// Token: 0x040000B3 RID: 179
	public AudioClip[] kissingClip;

	// Token: 0x040000B4 RID: 180
	public AudioClip swishClip;

	// Token: 0x040000B5 RID: 181
	private Animator animator;

	// Token: 0x040000B6 RID: 182
	private bool sceneReady = true;

	// Token: 0x040000B7 RID: 183
	private int currentScene;

	// Token: 0x040000B8 RID: 184
	public cutsceneElement[] sceneArray;
}
