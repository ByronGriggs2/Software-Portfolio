using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using AwariaInterface;
using Steamworks;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.SceneManagement;

// Token: 0x0200006A RID: 106
public class ManagerScript : MonoBehaviour
{
	// Token: 0x0600024C RID: 588 RVA: 0x0000A468 File Offset: 0x00008668
	private void Awake()
	{
		if (ManagerScript.instance == null)
		{
			ManagerScript.instance = this;
		}
		else if (ManagerScript.instance != this)
		{
			UnityEngine.Object.Destroy(base.gameObject);
			return;
		}
        if (MainInterface.Instance == null)
        {
            new GameObject("ByronModLoader").AddComponent<MainInterface>();
        }
        UnityEngine.Object.DontDestroyOnLoad(base.gameObject);
		this.directory = Directory.GetCurrentDirectory();
		this.menuTxt = File.ReadAllLines(this.directory + "/local/m.json");
		this.chapTxt = File.ReadAllLines(this.directory + "/local/ch.json");
		this.nextLevel = SceneManager.GetActiveScene().buildIndex;
		Cursor.visible = !Screen.fullScreen;
		Resolution[] resolutions = Screen.resolutions;
		Array.Reverse(resolutions);
        QualitySettings.vSyncCount = 0; // Byron's mod- allows us to force the framerate to be 60 fps
        int refreshRate = 60;
		bool flag = true;
		this.filteredRes = new List<Resolution>();
		for (int i = 0; i < resolutions.Length; i++)
		{
			if (resolutions[i].refreshRate == refreshRate)
			{
				this.filteredRes.Add(resolutions[i]);
				if (flag && this.filteredRes[this.filteredRes.Count - 1].width == Screen.width && this.filteredRes[this.filteredRes.Count - 1].height == Screen.height)
				{
					this.defaultResIndex = (this.currentResIndex = this.filteredRes.Count - 1);
					flag = false;
				}
			}
		}
		if (flag)
		{
			this.filteredRes.Insert(0, Screen.currentResolution);
			this.defaultResIndex = (this.currentResIndex = 0);
		}
	}

	// Token: 0x0600024D RID: 589 RVA: 0x0000A62C File Offset: 0x0000882C
	private void Start()
	{
		this.currentClips = new List<AudioSource>();
		this.VolumeChange("sfxVolume", PlayerPrefs.GetFloat("sfxVolume", 7f) / 10f);
		this.VolumeChange("musicVolume", PlayerPrefs.GetFloat("musicVolume", 5f) / 10f);
	}

	// Token: 0x0600024E RID: 590 RVA: 0x0000A684 File Offset: 0x00008884
	public void ResolutionPrinter(ButtonScript button, bool atEnable = false)
	{
		if (atEnable)
		{
			this.currentResIndex = this.defaultResIndex;
		}
		button.buttonText2.text = this.filteredRes[this.currentResIndex].width.ToString() + "x" + this.filteredRes[this.currentResIndex].height.ToString();
	}

	// Token: 0x0600024F RID: 591 RVA: 0x0000A6F8 File Offset: 0x000088F8
	public void ResolutionSet()
	{
		Screen.SetResolution(this.filteredRes[this.currentResIndex].width, this.filteredRes[this.currentResIndex].height, Screen.fullScreen);
		this.defaultResIndex = this.currentResIndex;
	}

	// Token: 0x06000250 RID: 592 RVA: 0x0000A750 File Offset: 0x00008950
	public void ResolutionSwitch(ButtonScript button, bool additive)
	{
		if (additive)
		{
			this.currentResIndex++;
			if (this.currentResIndex == this.filteredRes.Count)
			{
				this.currentResIndex = 0;
			}
		}
		else
		{
			this.currentResIndex--;
			if (this.currentResIndex < 0)
			{
				this.currentResIndex = this.filteredRes.Count - 1;
			}
		}
		this.ResolutionPrinter(button, false);
	}

	// Token: 0x06000251 RID: 593 RVA: 0x0000A7BC File Offset: 0x000089BC
	public void LevelComplete()
	{
		if (this.nextLevel < 13)
		{
			this.mainMenu = true;
			this.subMenu = 4;
			this.nextLevel++;
			if (this.nextLevel > PlayerPrefs.GetInt("lvl", 1))
			{
				PlayerPrefs.SetInt("lvl", this.nextLevel);
				return;
			}
		}
		else if (this.nextLevel == 13)
		{
			PlayerPrefs.SetInt("gallery", 1);
			if (this.steam && SteamManager.Initialized)
			{
				SteamUserStats.SetAchievement("achiev_e");
				SteamUserStats.StoreStats();
			}
			this.mainMenu = true;
			this.subMenu = 6;
			this.nextLevel++;
		}
	}

	// Token: 0x06000252 RID: 594 RVA: 0x0000347D File Offset: 0x0000167D
	public void Reload()
	{
		if (this.mainMenu)
		{
			SceneManager.LoadScene(0);
			return;
		}
		SceneManager.LoadScene(this.nextLevel);
	}

	// Token: 0x06000253 RID: 595 RVA: 0x00003499 File Offset: 0x00001699
	public void MusicCheck(AudioClip track)
	{
		if (this.requestedTrack != track)
		{
			this.requestedTrack = track;
			base.StopAllCoroutines();
			base.StartCoroutine(this.SongChanger(track));
		}
	}

	// Token: 0x06000254 RID: 596 RVA: 0x000034C4 File Offset: 0x000016C4
	public IEnumerator SongChanger(AudioClip m)
	{
		this.musicCredit = this.menuTxt[22] + " " + m.name;
		while (this.musicSource.volume > 0f)
		{
			this.musicSource.volume = this.musicSource.volume - 0.04f;
			yield return new WaitForSeconds(0.01f);
		}
		this.musicSource.volume = 1f;
		this.musicSource.clip = m;
		this.musicSource.Play();
		yield break;
	}

	// Token: 0x06000255 RID: 597 RVA: 0x0000A864 File Offset: 0x00008A64
	public AudioSource PlayShortSound(AudioClip s, int priority = 0)
	{
		if (this.backgroundSoundDisabled && priority == 0)
		{
			return null;
		}
		AudioSource audioSource = UnityEngine.Object.Instantiate<AudioSource>(this.soundPrefab, base.transform.position, Quaternion.identity);
		audioSource.clip = s;
		audioSource.Play();
		this.currentClips.Add(audioSource);
		return audioSource;
	}

	// Token: 0x06000256 RID: 598 RVA: 0x0000A8B4 File Offset: 0x00008AB4
	public AudioSource PlayShortCommonSound(AudioClip s)
	{
		if (this.backgroundSoundDisabled || this.commonsoundDelay)
		{
			return null;
		}
		this.commonsoundDelay = true;
		AudioSource audioSource = UnityEngine.Object.Instantiate<AudioSource>(this.soundPrefab, base.transform.position, Quaternion.identity);
		audioSource.clip = s;
		audioSource.Play();
		this.currentClips.Add(audioSource);
		base.StartCoroutine(this.CommonsoundWaiter());
		return audioSource;
	}

	// Token: 0x06000257 RID: 599 RVA: 0x000034DA File Offset: 0x000016DA
	public IEnumerator CommonsoundWaiter()
	{
		yield return new WaitForSeconds(0.01f);
		this.commonsoundDelay = false;
		yield break;
	}

	// Token: 0x06000258 RID: 600 RVA: 0x0000A920 File Offset: 0x00008B20
	public void AudioPause(bool stop)
	{
		if (stop)
		{
			for (int i = 0; i < this.currentClips.Count; i++)
			{
				if (this.currentClips[i] == null)
				{
					this.currentClips.RemoveAt(i);
					i--;
				}
				else
				{
					this.currentClips[i].Pause();
				}
			}
			return;
		}
		for (int j = 0; j < this.currentClips.Count; j++)
		{
			if (this.currentClips[j] == null)
			{
				this.currentClips.RemoveAt(j);
				j--;
			}
			else
			{
				this.currentClips[j].UnPause();
			}
		}
	}

	// Token: 0x06000259 RID: 601 RVA: 0x000034E9 File Offset: 0x000016E9
	public void AudioRemoveAll()
	{
		this.currentClips.Clear();
		this.backgroundSoundDisabled = true;
	}

	// Token: 0x0600025A RID: 602 RVA: 0x000034FD File Offset: 0x000016FD
	public void AudioRemover(AudioSource source)
	{
		this.currentClips.Remove(source);
	}

	// Token: 0x0600025B RID: 603 RVA: 0x0000A9CC File Offset: 0x00008BCC
	public void PlayLongSound(AudioClip s)
	{
		this.audioLongArray[this.audioLongIndex].clip = s;
		this.audioLongArray[this.audioLongIndex].Play();
		this.audioLongIndex++;
		if (this.audioLongIndex == this.audioLongArray.Length)
		{
			this.audioLongIndex = 0;
		}
	}

	// Token: 0x0600025C RID: 604 RVA: 0x0000AA24 File Offset: 0x00008C24
	public void PlayPausedSound(AudioClip s)
	{
		this.audioShortArray[this.audioIndex].clip = s;
		this.audioShortArray[this.audioIndex].Play();
		this.audioIndex++;
		if (this.audioIndex == this.audioShortArray.Length)
		{
			this.audioIndex = 0;
		}
	}

	// Token: 0x0600025D RID: 605 RVA: 0x0000350C File Offset: 0x0000170C
	public void VolumeChange(string soundType, float volume)
	{
		if (volume == 0f)
		{
			volume = 0.0001f;
		}
		this.mixer.SetFloat(soundType, Mathf.Log10(volume) * 20f);
	}

	// Token: 0x0600025E RID: 606 RVA: 0x00003536 File Offset: 0x00001736
	public void MusicMuffle(int muffle = 20)
	{
		this.VolumeChange("musicVolume", PlayerPrefs.GetFloat("musicVolume", 5f) / (float)muffle);
	}

	// Token: 0x04000240 RID: 576
	public static ManagerScript instance;

	// Token: 0x04000241 RID: 577
	public string directory;

	// Token: 0x04000242 RID: 578
	public string[] menuTxt;

	// Token: 0x04000243 RID: 579
	public string[] chapTxt;

	// Token: 0x04000244 RID: 580
	public bool firstTransition = true;

	// Token: 0x04000245 RID: 581
	public AudioMixer mixer;

	// Token: 0x04000246 RID: 582
	public string musicCredit;

	// Token: 0x04000247 RID: 583
	public bool mainMenu = true;

	// Token: 0x04000248 RID: 584
	public int subMenu = 1;

	// Token: 0x04000249 RID: 585
	public int nextLevel;

	// Token: 0x0400024A RID: 586
	public bool introReady;

	// Token: 0x0400024B RID: 587
	private List<Resolution> filteredRes;

	// Token: 0x0400024C RID: 588
	private int currentResIndex;

	// Token: 0x0400024D RID: 589
	private int defaultResIndex;

	// Token: 0x0400024E RID: 590
	public AudioSource musicSource;

	// Token: 0x0400024F RID: 591
	public AudioSource soundPrefab;

	// Token: 0x04000250 RID: 592
	public AudioSource[] audioLongArray;

	// Token: 0x04000251 RID: 593
	private int audioLongIndex;

	// Token: 0x04000252 RID: 594
	public AudioSource[] audioShortArray;

	// Token: 0x04000253 RID: 595
	private int audioIndex;

	// Token: 0x04000254 RID: 596
	private AudioClip requestedTrack;

	// Token: 0x04000255 RID: 597
	private List<AudioSource> currentClips;

	// Token: 0x04000256 RID: 598
	public bool backgroundSoundDisabled;

	// Token: 0x04000257 RID: 599
	private bool commonsoundDelay;

	// Token: 0x04000258 RID: 600
	public bool steam;

	// Token: 0x04000259 RID: 601
	public bool resetableInput;
}
