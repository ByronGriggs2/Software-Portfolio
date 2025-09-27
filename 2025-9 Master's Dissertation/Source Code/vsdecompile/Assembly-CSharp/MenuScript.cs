using System;
using System.Collections;
using AwariaInterface;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.InputSystem;
using UnityEngine.UI;

// Token: 0x02000079 RID: 121
public class MenuScript : MonoBehaviour
{
	private AwariaInterface.IInputProvider byronInputs;
	// Token: 0x0600029A RID: 666 RVA: 0x0000B408 File Offset: 0x00009608
	private void Start()
	{
		if (MainInterface.Instance == null)
		{
			new GameObject("ByronModLoader").AddComponent<MainInterface>();
		}
        this.byronInputs = GameObject.FindObjectOfType<AwariaInterface.MainInterface>() as AwariaInterface.IInputProvider;
        if (ManagerScript.instance.resetableInput)
		{
			ManagerScript.instance.resetableInput = false;
			foreach (InputActionMap actions in this.inputActions.actionMaps)
			{
				actions.RemoveAllBindingOverrides();
			}
			string value = this.inputActions.SaveBindingOverridesAsJson();
			PlayerPrefs.SetString("rebinds", value);
		}
		string @string = PlayerPrefs.GetString("rebinds");
		if (!string.IsNullOrEmpty(@string))
		{
			this.inputActions.LoadBindingOverridesFromJson(@string, true);
		}
		if (ManagerScript.instance.mainMenu)
		{
			if (PlayerPrefs.GetInt("gallery", 0) == 1)
			{
				this.galleryUnlocked = true;
			}
			ManagerScript.instance.introReady = true;
			ManagerScript.instance.mainMenu = false;
			this.submenuArray[2].parentMenu = 1;
			this.MenuOpenClose(1, true);
			int subMenu = ManagerScript.instance.subMenu;
			if (subMenu != 1)
			{
				this.MenuSwitch(subMenu);
				ManagerScript.instance.subMenu = 1;
				return;
			}
			ManagerScript.instance.nextLevel = 0;
            MainInterface.Instance.setCurrentLevel(0);
        }
	}

	// Token: 0x0600029B RID: 667 RVA: 0x0000B548 File Offset: 0x00009748
	private void Update()
	{
        if (this.player.inputsBlocked)
		{
			return;
		}
		if (!this.player.inMenu)
		{
			if (byronInputs.getCancel())
			{
				this.MenuOpenClose(0, true);
				ManagerScript.instance.PlayPausedSound(this.openClip);
				return;
			}
		}
		else if (this.player.inMenu)
		{
			if (byronInputs.getCancel() || byronInputs.getSecondary())
			{
				if (this.currentMenu == 0)
				{
					this.MenuOpenClose(0, false);
				}
				else if (this.currentMenu != 1)
				{
					this.MenuSwitch(this.submenuArray[this.currentMenu].parentMenu);
				}
				ManagerScript.instance.PlayPausedSound(this.closeClip);
			}
			if (this.submenuArray[this.currentMenu].inputMenu)
			{
				return;
			}
			if ((double)byronInputs.getMoveVector().y < -0.1)
			{
				if (this.CheatCheck("d"))
				{
					return;
				}
				this.NextButton(true);
				return;
			}
			else if ((double)byronInputs.getMoveVector().y > 0.1)
			{
				if (this.CheatCheck("u"))
				{
					return;
				}
				this.NextButton(false);
				return;
			}
			else if ((double)byronInputs.getMoveVector().x < -0.1)
			{
				if (this.CheatCheck("l"))
				{
					return;
				}
				if (this.submenuArray[this.currentMenu].unusualNavigation)
				{
					this.NextButton(false);
					return;
				}
				this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonSwitch(false);
				return;
			}
			else if ((double)byronInputs.getMoveVector().x > 0.1)
			{
				if (this.CheatCheck("r"))
				{
					return;
				}
				if (this.submenuArray[this.currentMenu].unusualNavigation)
				{
					this.NextButton(true);
					return;
				}
				this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonSwitch(true);
				return;
			}
			else
			{
				this.dirdown = false;
			}
		}
	}

	// Token: 0x0600029C RID: 668 RVA: 0x0000B74C File Offset: 0x0000994C
	public void InputConfirm()
	{
		if (this.player.inputsBlocked || this.submenuArray[this.currentMenu].inputMenu)
		{
			return;
		}
		this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonConfirm();
	}

	// Token: 0x0600029D RID: 669 RVA: 0x0000B79C File Offset: 0x0000999C
	private void NextButton(bool additive)
	{
		if (this.submenuArray[this.currentMenu].maxIndex == 1)
		{
			return;
		}
		this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonHighlight(false, true);
		if (additive)
		{
			if (this.currentMenu == 1 && this.currentIndex == 0 && !this.galleryUnlocked)
			{
				this.currentIndex = 2;
			}
			else
			{
				this.currentIndex++;
				if (this.currentIndex == this.submenuArray[this.currentMenu].maxIndex)
				{
					this.currentIndex = 0;
				}
			}
		}
		else if (this.currentMenu == 1 && this.currentIndex == 2 && !this.galleryUnlocked)
		{
			this.currentIndex = 0;
		}
		else
		{
			this.currentIndex--;
			if (this.currentIndex < 0)
			{
				this.currentIndex = this.submenuArray[this.currentMenu].maxIndex - 1;
			}
		}
		this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonHighlight(true, false);
		if (this.currentMenu == 7)
		{
			ManagerScript.instance.PlayPausedSound(this.hoverClip[2]);
			return;
		}
		if (this.currentMenu == 4)
		{
			ManagerScript.instance.PlayPausedSound(this.hoverClip[1]);
			return;
		}
		ManagerScript.instance.PlayPausedSound(this.hoverClip[0]);
	}

	// Token: 0x0600029E RID: 670 RVA: 0x0000B8F8 File Offset: 0x00009AF8
	private bool CheatCheck(string dir)
	{
		if (this.PressCheck(ref this.dirdown))
		{
			return true;
		}
		if (this.currentMenu != 6)
		{
			return false;
		}
		if (dir == this.cheatsheet[this.currentCheat])
		{
			this.currentCheat++;
		}
		else if (dir == "d")
		{
			this.currentCheat = 1;
		}
		else
		{
			this.currentCheat = 0;
		}
		if (this.currentCheat == this.cheatsheet.Length)
		{
			this.pentaObject.SetActive(true);
			ManagerScript.instance.PlayPausedSound(this.openClip);
			PlayerPrefs.SetInt("gallery", 1);
			this.galleryUnlocked = true;
			PlayerPrefs.SetInt("beel", 1);
			PlayerPrefs.SetInt("lvl", 13);
		}
		else
		{
			ManagerScript.instance.PlayPausedSound(this.hoverClip[0]);
		}
		return true;
	}

	// Token: 0x0600029F RID: 671 RVA: 0x000037E6 File Offset: 0x000019E6
	private bool PressCheck(ref bool press)
	{
		if (press)
		{
			return true;
		}
		press = true;
		return false;
	}

	// Token: 0x060002A0 RID: 672 RVA: 0x0000B9CC File Offset: 0x00009BCC
	public void MenuOpenClose(int menuNumber, bool open = true)
	{
		this.currentMenu = menuNumber;
		this.player.inMenu = open;
		this.TotalRefresh(this.submenuArray[this.currentMenu].bar, open);
		this.submenuArray[this.currentMenu].submenuCore.SetActive(open);
		if (open)
		{
			this.currentIndex = 0;
		}
		this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonHighlight(open, false);
		if (open)
		{
			Time.timeScale = 0f;
		}
		else
		{
			Time.timeScale = 1f;
		}
		ManagerScript.instance.AudioPause(open);
	}

	// Token: 0x060002A1 RID: 673 RVA: 0x0000BA6C File Offset: 0x00009C6C
	public void TotalRefresh(bool needed, bool open)
	{
		if (open)
		{
			this.submenuArray[this.currentMenu].MenuRefresh();
		}
		if (needed && open)
		{
			this.musicCredit.text = ManagerScript.instance.musicCredit;
			this.barObject.SetActive(true);
			this.barImage.color = this.submenuArray[this.currentMenu].barColor;
			this.barText.text = ManagerScript.instance.menuTxt[this.submenuArray[this.currentMenu].barName];
			return;
		}
		this.barObject.SetActive(false);
	}

	// Token: 0x060002A2 RID: 674 RVA: 0x0000BB08 File Offset: 0x00009D08
	public void MenuSwitch(int nextMenu)
	{
		this.submenuArray[this.currentMenu].submenuCore.SetActive(false);
		this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonHighlight(false, false);
		if (this.submenuArray[this.currentMenu].inputMenu)
		{
			EventSystem.current.SetSelectedGameObject(null);
		}
		this.currentIndex = 0;
		this.currentMenu = nextMenu;
		this.TotalRefresh(this.submenuArray[this.currentMenu].bar, true);
		if (this.submenuArray[this.currentMenu].aniTrigger != "")
		{
			this.animator.SetTrigger(this.submenuArray[this.currentMenu].aniTrigger);
		}
		this.submenuArray[this.currentMenu].submenuCore.SetActive(true);
		if (this.currentMenu == 4)
		{
			if (ManagerScript.instance.nextLevel != 0)
			{
				this.currentIndex = ManagerScript.instance.nextLevel - 1;
			}
			else
			{
				this.currentIndex = this.submenuArray[this.currentMenu].maxIndex - 1;
			}
		}
		else if (this.currentMenu == 7)
		{
			this.currentIndex = PlayerPrefs.GetInt("mode", 1);
			this.chapterImage.sprite = this.chapterImageArray[ManagerScript.instance.nextLevel - 1];
		}
		else if (this.currentMenu == 1)
		{
			ManagerScript.instance.nextLevel = 0;
            MainInterface.Instance.setCurrentLevel(0);
        }
		this.submenuArray[this.currentMenu].buttonArray[this.currentIndex].ButtonHighlight(true, false);
		if (this.submenuArray[this.currentMenu].inputMenu)
		{
			EventSystem.current.SetSelectedGameObject(this.submenuArray[this.currentMenu].firstInput);
		}
	}

	// Token: 0x060002A3 RID: 675 RVA: 0x000037F2 File Offset: 0x000019F2
	public void NextLevel(float delay = 0f)
	{
		this.player.inputsBlocked = true;
		base.StartCoroutine(this.TransitionDelay(delay));
	}

	// Token: 0x060002A4 RID: 676 RVA: 0x0000380E File Offset: 0x00001A0E
	private IEnumerator TransitionDelay(float delay)
	{
		yield return new WaitForSeconds(delay);
		this.tranScript.Transition();
		yield break;
	}

	// Token: 0x060002A5 RID: 677 RVA: 0x0000BCCC File Offset: 0x00009ECC
	public void ResetControls()
	{
		foreach (InputActionMap inputActionMap in this.inputActions.actionMaps)
		{
			foreach (InputAction action in inputActionMap.actions)
			{
				action.RemoveBindingOverride(InputBinding.MaskByGroup(this.submenuArray[this.currentMenu].controlName));
			}
		}
	}

	// Token: 0x04000289 RID: 649
	public PlayerScript player;

	// Token: 0x0400028A RID: 650
	public GameObject barObject;

	// Token: 0x0400028B RID: 651
	public Image barImage;

	// Token: 0x0400028C RID: 652
	public TMP_Text barText;

	// Token: 0x0400028D RID: 653
	public TMP_Text musicCredit;

	// Token: 0x0400028E RID: 654
	public SubmenuScript[] submenuArray;

	// Token: 0x0400028F RID: 655
	public Animator animator;

	// Token: 0x04000290 RID: 656
	public TransitionScript tranScript;

	// Token: 0x04000291 RID: 657
	public InputActionAsset inputActions;

	// Token: 0x04000292 RID: 658
	public Sprite[] chapterImageArray;

	// Token: 0x04000293 RID: 659
	public Image chapterImage;

	// Token: 0x04000294 RID: 660
	public string[] galleryTxt;

	// Token: 0x04000295 RID: 661
	public AudioClip menuMusic;

	// Token: 0x04000296 RID: 662
	public AudioClip openClip;

	// Token: 0x04000297 RID: 663
	public AudioClip closeClip;

	// Token: 0x04000298 RID: 664
	public AudioClip[] hoverClip;

	// Token: 0x04000299 RID: 665
	public GameObject pentaObject;

	// Token: 0x0400029A RID: 666
	public int currentMenu;

	// Token: 0x0400029B RID: 667
	private string[] cheatsheet = new string[]
	{
		"d",
		"u",
		"r",
		"u",
		"l",
		"d",
		"d",
		"l",
		"u",
		"d",
		"r",
		"u"
	};

	// Token: 0x0400029C RID: 668
	private int currentIndex;

	// Token: 0x0400029D RID: 669
	private int currentCheat;

	// Token: 0x0400029E RID: 670
	private bool dirdown;

	// Token: 0x0400029F RID: 671
	public bool galleryUnlocked;
}
