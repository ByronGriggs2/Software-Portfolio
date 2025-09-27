using System;
using System.Collections;
using System.Collections.Generic;
using Steamworks;
using TMPro;
using UnityEngine;

// Token: 0x02000067 RID: 103
public class RotationScript : MonoBehaviour
{
	// Token: 0x06000232 RID: 562
	private void Start()
	{
		this.locationDelay = base.transform;
		this.xFlipDelay = 1;
		this.aniDelay = this.droneAnimator;
		this.difficulty = (float)PlayerPrefs.GetInt("mode", 0);
		if (this.difficulty == 2f)
		{
			this.droneState = 2;
			this.droneAnimator.gameObject.SetActive(false);
		}
		this.totalEngines = this.goodEngines.Count;
		this.blackout = base.GetComponent<SpriteRenderer>();
		this.scoreText.text = this.repairCount.ToString();
		this.capText.text = "   / " + this.repairGoal.ToString();
		if (this.boss.Length == 3 && ManagerScript.instance.introReady)
		{
			this.dashTutorial.SetActive(true);
			this.dashCount = 1;
			this.dashText.text = string.Concat(new string[]
			{
				ManagerScript.instance.menuTxt[20],
				" ",
				(this.dashCount - 1).ToString(),
				" / 4 ",
				ManagerScript.instance.menuTxt[21]
			});
		}
        AwariaInterface.MainInterface.Instance.setupGenerators();
    }

    // Token: 0x06000233 RID: 563
    public void DashCounter()
	{
		this.dashCount++;
		if (this.dashCount == 5)
		{
			ManagerScript.instance.introReady = false;
			this.dashTutorial.SetActive(false);
			return;
		}
		this.dashText.text = string.Concat(new string[]
		{
			ManagerScript.instance.menuTxt[20],
			" ",
			(this.dashCount - 1).ToString(),
			" / 4 ",
			ManagerScript.instance.menuTxt[21]
		});
	}

	// Token: 0x06000234 RID: 564
	public void GameStarter()
	{
		base.StartCoroutine(this.Rotation());
	}

	// Token: 0x06000235 RID: 565
	private IEnumerator Rotation()
	{
		if (!this.calmLevel)
		{
			yield return new WaitForSeconds(0.5f);
			if (this.bigStriga != null)
			{
				this.bigStriga.BossStart();
			}
			else
			{
				this.GhostIntro();
			}
		}
		while (!this.gameEnded && this.defectCount < this.repairGoal)
		{
			yield return new WaitForSeconds(0.5f);
			if (this.goodEngines.Count > this.totalEngines - this.defectCap)
			{
				this.DefectCreator();
			}
			yield return new WaitForSeconds(2.5f);
		}
		yield break;
	}

	// Token: 0x06000236 RID: 566
	public void DefectCreator()
	{
		int index = UnityEngine.Random.Range(0, this.goodEngines.Count);
		EngineScript engineScript = this.goodEngines[index];
		this.goodEngines.RemoveAt(index);
		this.randomDefect[0] = this.RandomizedWant();
		if (this.defectCount >= this.doubleAfter)
		{
			this.randomDefect[1] = this.RandomizedWant();
		}
		else
		{
			this.randomDefect[1] = 10;
		}
		if (this.randomDefect[0] == this.randomDefect[1])
		{
			this.DuplicateRemover();
		}
		this.defectCount++;
		if (this.difficulty == 0f)
		{
			engineScript.DefectStart(this.randomDefect, 99f);
		}
		else
		{
			engineScript.DefectStart(this.randomDefect, this.baseTime - (float)this.repairCount - this.difficulty * 10f + 10f);
		}
		this.camScript.Shakedown(0.2f, 0.2f);
	}

	// Token: 0x06000237 RID: 567
	private int RandomizedWant()
	{
		int result;
		if (this.junkPoolOverride.Count == 0)
		{
			int index = UnityEngine.Random.Range(0, this.junkPool.Count);
			result = this.junkPool[index];
			this.junkPool.RemoveAt(index);
		}
		else
		{
			result = this.junkPoolOverride[0];
			this.junkPoolOverride.RemoveAt(0);
		}
		return result;
	}

	// Token: 0x06000238 RID: 568
	private void DuplicateRemover()
	{
		int item = this.randomDefect[1];
		this.randomDefect[1] = this.RandomizedWant();
		if (this.randomDefect[0] == this.randomDefect[1])
		{
			this.DuplicateRemover();
		}
		this.junkPool.Add(item);
	}

	// Token: 0x06000239 RID: 569
	public void RepairFinished(EngineScript customer)
	{
		this.goodEngines.Add(customer);
		if (this.defectCap == 1 && this.repairCount >= this.twoGenAfter)
		{
			this.defectCap++;
		}
		else if (this.defectCap == 2 && this.repairCount >= this.threeGenAfter)
		{
			this.defectCap++;
		}
		if (this.repairCount == 1 && this.calmLevel)
		{
			this.GhostIntro();
			return;
		}
		if (this.soloHaunt && this.boss.Length > 1)
		{
			this.soloHaunt = false;
			for (int i = 1; i < this.boss.Length; i++)
			{
				this.boss[i].Spawn();
			}
		}
	}

	// Token: 0x0600023A RID: 570
	public void GhostIntro()
	{
		if (this.gameEnded)
		{
			return;
		}
		if (this.upscaleAniScript.droneHit)
		{
			this.delayState = 1;
			return;
		}
		if (ManagerScript.instance.introReady && this.hasIntro)
		{
			ManagerScript.instance.introReady = false;
			this.playerScript.inputsBlocked = true;
			this.upscaleAniScript.Activator("introStart", base.transform, 1);
			Time.timeScale = 0f;
		}
		this.soloHaunt = true;
		this.boss[0].Spawn();
	}

	// Token: 0x0600023B RID: 571
	public void ScoreUpdate(Transform gen, Animator genAnim, int xFlip = 1)
	{
		if (this.gameEnded)
		{
			return;
		}
		if (this.upscaleAniScript.droneHit && this.repairCount + 1 == this.repairGoal)
		{
			this.delayState = 3;
			this.locationDelay = gen;
			this.aniDelay = genAnim;
			this.xFlipDelay = xFlip;
			return;
		}
		this.repairCount++;
		this.scoreText.text = this.repairCount.ToString();
		if (this.repairCount == this.repairGoal)
		{
			AwariaInterface.MainInterface.Instance.levelEndCleanup();
			if (this.difficulty == 2f)
			{
				string key = ManagerScript.instance.nextLevel.ToString() + "H";
				if (PlayerPrefs.GetInt(key, 0) == 0)
				{
					PlayerPrefs.SetInt(key, 1);
					PlayerPrefs.SetInt("hardCount", PlayerPrefs.GetInt("hardCount", 0) + 1);
				}
				if (PlayerPrefs.GetInt("hardCount", 0) == 13 && ManagerScript.instance.steam && SteamManager.Initialized)
				{
					SteamUserStats.SetAchievement("achiev_h");
					SteamUserStats.StoreStats();
				}
			}
			this.waveSprite.localScale = new Vector3((float)xFlip, 1f, 1f);
			this.playerScript.inputsBlocked = true;
			this.gameEnded = true;
			ManagerScript.instance.PlayLongSound(this.victoryClip);
			this.upscaleAniScript.Activator("genFix", gen, 1);
			this.upscaleAniScript.genAnim = genAnim;
			ManagerScript.instance.AudioPause(true);
			ManagerScript.instance.AudioRemoveAll();
			Time.timeScale = 0f;
			return;
		}
		genAnim.SetTrigger("fixed");
	}

	// Token: 0x0600023C RID: 572
	public void Gameover(Transform location, bool dead, int xFlip = 1)
	{
        AwariaInterface.MainInterface.Instance.levelEndCleanup();
        AwariaInterface.MainInterface.Instance.setPlayerDeath();
        if (this.gameEnded)
		{
			return;
		}
		if (dead)
		{
			if (this.playerScript.dashing || this.droneState == 1)
			{
				return;
			}
		}
		else if (this.upscaleAniScript.droneHit)
		{
			this.delayState = 2;
			this.locationDelay = location;
			this.deadDelay = dead;
			this.xFlipDelay = xFlip;
			return;
		}
		if (dead)
		{
			ManagerScript.instance.PlayLongSound(this.dmgAudio[UnityEngine.Random.Range(0, this.dmgAudio.Length)]);
		}
		Time.timeScale = 0f;
		ManagerScript.instance.AudioPause(true);
		this.playerScript.inputsBlocked = true;
		if (dead && this.droneState == 0)
		{
			this.upscaleAniScript.droneHit = true;
			this.droneState++;
			base.StartCoroutine(this.DroneRegen());
			this.upscaleAniScript.Activator("droneDeath", this.playerScript.transform, 1);
			return;
		}
		this.blackout.enabled = true;
		this.gameEnded = true;
		ManagerScript.instance.AudioRemoveAll();
		if (dead)
		{
			this.upscaleAniScript.Activator("charaDeath", location, 1);
			return;
		}
		this.upscaleAniScript.Activator("genDeath", location, xFlip);
	}

	// Token: 0x0600023D RID: 573
	public void AntiDronebug()
	{
		if (this.delayState == 0)
		{
			return;
		}
		if (this.delayState == 1)
		{
			this.GhostIntro();
		}
		else if (this.delayState == 2)
		{
			this.Gameover(this.locationDelay, this.deadDelay, this.xFlipDelay);
		}
		else if (this.delayState == 3)
		{
			this.ScoreUpdate(this.locationDelay, this.aniDelay, this.xFlipDelay);
		}
		this.delayState = 0;
	}

	// Token: 0x0600023E RID: 574
	private IEnumerator DroneRegen()
	{
		yield return new WaitForSeconds(0.01f);
		this.droneAnimator.SetTrigger("death");
		if (this.electroWipe != null)
		{
			ManagerScript.instance.PlayShortSound(this.electroWipeClip, 0);
			this.electroWipe.Activate(this.playerScript.transform.position);
		}
		yield return new WaitForSeconds(0.3f);
		ManagerScript.instance.PlayShortSound(this.droneDeathClip, 0);
		yield return new WaitForSeconds(0.5f);
		this.droneState++;
		yield return new WaitForSeconds(2.7f + 6f * this.difficulty);
		this.droneAnimator.SetTrigger("start");
		this.playerScript.droneHolo.SetTrigger("appear");
		ManagerScript.instance.PlayShortSound(this.droneRepairAudio, 0);
		base.StartCoroutine(this.playerScript.MaterialSwapper());
		this.droneState = 0;
		yield break;
	}

	// Token: 0x0400020F RID: 527
	private SpriteRenderer blackout;

	// Token: 0x04000210 RID: 528
	public List<EngineScript> goodEngines;

	// Token: 0x04000211 RID: 529
	public List<int> junkPool;

	// Token: 0x04000212 RID: 530
	public List<int> junkPoolOverride;

	// Token: 0x04000213 RID: 531
	public TMP_Text scoreText;

	// Token: 0x04000214 RID: 532
	public TMP_Text capText;

	// Token: 0x04000215 RID: 533
	public GhostAiScript[] boss;

	// Token: 0x04000216 RID: 534
	public PlayerScript playerScript;

	// Token: 0x04000217 RID: 535
	public UpscaleAnimatorScript upscaleAniScript;

	// Token: 0x04000218 RID: 536
	public CameraScript camScript;

	// Token: 0x04000219 RID: 537
	public Transform waveSprite;

	// Token: 0x0400021A RID: 538
	public int droneState;

	// Token: 0x0400021B RID: 539
	public Animator droneAnimator;

	// Token: 0x0400021C RID: 540
	public AudioClip[] dmgAudio;

	// Token: 0x0400021D RID: 541
	public AudioClip droneRepairAudio;

	// Token: 0x0400021E RID: 542
	public AudioClip victoryClip;

	// Token: 0x0400021F RID: 543
	public AudioClip electroWipeClip;

	// Token: 0x04000220 RID: 544
	public AudioClip droneDeathClip;

	// Token: 0x04000221 RID: 545
	public BigStrigaScript bigStriga;

	// Token: 0x04000222 RID: 546
	public GameObject dashTutorial;

	// Token: 0x04000223 RID: 547
	public TMP_Text dashText;

	// Token: 0x04000224 RID: 548
	public int dashCount = 5;

	// Token: 0x04000225 RID: 549
	public ElectroWipeScript electroWipe;

	// Token: 0x04000226 RID: 550
	public int repairGoal = 8;

	// Token: 0x04000227 RID: 551
	public int doubleAfter;

	// Token: 0x04000228 RID: 552
	public int twoGenAfter;

	// Token: 0x04000229 RID: 553
	public int threeGenAfter;

	// Token: 0x0400022A RID: 554
	public float baseTime = 40f;

	// Token: 0x0400022B RID: 555
	public bool hasIntro;

	// Token: 0x0400022C RID: 556
	public bool calmLevel = true;

	// Token: 0x0400022D RID: 557
	public int defectCap = 1;

	// Token: 0x0400022E RID: 558
	public bool soloHaunt;

	// Token: 0x0400022F RID: 559
	private int repairCount;

	// Token: 0x04000230 RID: 560
	private int defectCount;

	// Token: 0x04000231 RID: 561
	private int[] randomDefect = new int[]
	{
		10,
		10
	};

	// Token: 0x04000232 RID: 562
	private bool gameEnded;

	// Token: 0x04000233 RID: 563
	private int totalEngines;

	// Token: 0x04000234 RID: 564
	private float difficulty;

	// Token: 0x04000235 RID: 565
	private int delayState;

	// Token: 0x04000236 RID: 566
	private Transform locationDelay;

	// Token: 0x04000237 RID: 567
	private bool deadDelay;

	// Token: 0x04000238 RID: 568
	private int xFlipDelay;

	// Token: 0x04000239 RID: 569
	private Animator aniDelay;
}
