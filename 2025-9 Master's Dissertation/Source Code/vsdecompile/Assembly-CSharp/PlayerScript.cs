using System;
using System.Collections;
using AwariaInterface;
using Steamworks;
using UnityEngine;

// Token: 0x0200007E RID: 126
public class PlayerScript : MonoBehaviour
{
	private IInputProvider byronInputs;
	// Token: 0x060002B8 RID: 696
	private void Start()
	{
		if (MainInterface.Instance == null)
		{
			new GameObject("ByronModLoader").AddComponent<MainInterface>();
		}
		if (this.moveSpeed == 8f)
		{
			this.babeczkiState = 1;
		}
		this.spriteMaterial = this.bodyparts[0].material;
		this.hand = new int[2];
		this.array = new int[]
		{
			0,
			1
		};
		this.handspriteArray = new Sprite[][]
		{
			this.fronthandSpriteArray,
			this.backhandSpriteArray
		};
		this.rb = base.GetComponent<Rigidbody2D>();
		this.animator = base.GetComponent<Animator>();
		this.playerHitbox = base.GetComponent<CircleCollider2D>();
		base.InvokeRepeating("TargetUpdate", 0.05f, 0.05f);

		this.byronInputs = GameObject.FindObjectOfType<AwariaInterface.MainInterface>() as AwariaInterface.IInputProvider;
	}

	public Vector3 byronGetPlayerPosition()
	{
		return this.gameObject.transform.position;
	}
	public int[] byronGetHands()
	{
		return this.hand;
	}
	public bool byronGetDash()
	{
		return !this.inputsBlocked && (this.movementMode == 0 || (this.movementMode == -2 && this.superDash == 3));
    }
	public Vector2 byronGetMoveDir()
	{
		return this.moveDir;
	}

	// Token: 0x060002B9 RID: 697
	private void Update()
    {
        if (this.inMenu || this.inCutscene)
		{
			return;
		}
		if (!this.inputsBlocked)
		{
			float num = byronInputs.getMoveVector().x;
			float num2 = byronInputs.getMoveVector().y;
			if ((double)num < 0.08 && (double)num > -0.08)
			{
				num = 0f;
			}
			if ((double)num2 < 0.08 && (double)num2 > -0.08)
			{
				num2 = 0f;
			}
			this.moveDirNew = new Vector2(num, num2).normalized;
			if (this.moveDirNew != Vector2.zero)
			{
				if (this.movementMode < 2 && this.movementMode != -2)
				{
					this.secondaryDashDir = (this.dashDir = this.moveDirNew);
				}
				else
				{
					this.secondaryDashDir = this.moveDirNew;
				}
			}
			if (byronInputs.getSecondary())
			{
				if (this.movementMode == 0)
				{
					this.currentDash = base.StartCoroutine(this.Dash(3));
				}
				else if (this.movementMode == -2 && this.superDash == 3)
				{
					base.StopCoroutine(this.currentDash);
					this.currentDash = base.StartCoroutine(this.Dash(4));
				}
				else if (this.babeczkiState == 1 && this.moveDirNew == Vector2.zero)
				{
					if (this.babeczkiRoutine != null)
					{
						base.StopCoroutine(this.babeczkiRoutine);
					}
					this.babeczkiState = 2;
					this.moveDir = this.moveDirNew;
					this.animator.SetTrigger("isEating");
					this.EatEnder();
					this.eatSource = ManagerScript.instance.PlayShortSound(this.eatClip, 0);
					this.babeczkiRoutine = base.StartCoroutine(this.EatCooldown());
					this.babeczkiCount++;
					if (this.babeczkiCount == 9 && ManagerScript.instance.steam && SteamManager.Initialized)
					{
						SteamUserStats.SetAchievement("achiev_b");
						SteamUserStats.StoreStats();
					}
				}
			}
		}
		else
		{
			this.moveDirNew = new Vector2(0f, 0f);
		}
		// TEMPORARILY DISABLED AS A TEST - BYRON
		//this.moveDrag = false;

		if (this.moveDrag && this.moveDir != this.moveDirNew)
		{
			float maxDelta = 10f / this.chargeEase * Time.deltaTime;
			this.moveDir.x = Mathf.MoveTowards(this.moveDir.x, this.moveDirNew.x, maxDelta);
			this.moveDir.y = Mathf.MoveTowards(this.moveDir.y, this.moveDirNew.y, maxDelta);
			return;
		}
		this.moveDir = this.moveDirNew;
	}

	// Token: 0x060002BA RID: 698
	private IEnumerator EatCooldown()
	{
		yield return new WaitForSeconds(0.7f);
		this.babeczkiState = 1;
		yield break;
	}

	// Token: 0x060002BB RID: 699
	public void EatEnder()
	{
		if (this.eatSource != null)
		{
			UnityEngine.Object.Destroy(this.eatSource.gameObject);
		}
	}

	// Token: 0x060002BC RID: 700
	public void PostEat()
	{
		base.StartCoroutine(this.EnergyBoost());
	}

	// Token: 0x060002BD RID: 701
	private IEnumerator EnergyBoost()
	{
		this.moveSpeed += 1f;
		yield return new WaitForSeconds(2f);
		this.moveSpeed -= 1f;
		yield break;
	}

	// Token: 0x060002BE RID: 702
	private IEnumerator Dash(int dashType = 3)
	{
		this.dashing = true;
		this.EatEnder();
		if (dashType == 3)
		{
			if (!this.gameBegan && this.rotationScript.dashCount < 5)
			{
				ManagerScript.instance.PlayShortSound(this.tutorialClips[this.rotationScript.dashCount], 0);
				this.rotationScript.DashCounter();
			}
			else if (this.superDash == 0)
			{
				ManagerScript.instance.PlayShortSound(this.dashClips[UnityEngine.Random.Range(0, this.dashClips.Length)], 0);
			}
            this.movementMode = 1;
            yield return new WaitForSeconds(0.05f);
		}
		if (this.superDash != 0)
		{
			ManagerScript.instance.PlayShortSound(this.superdashClips[UnityEngine.Random.Range(0, this.superdashClips.Length)], 0);
			this.superDash = 2;
			base.StartCoroutine(this.MaterialSwapper());
			this.moveSpeed = 14f;
			this.playerHitbox.radius = 1f;
			this.dashDir = this.secondaryDashDir;
			this.shock[dashType - 3].Emit(base.transform.position, true, this.dashDir);
			this.dustObject.transform.localScale = this.spriteTransform.localScale;
		}
		this.dustObject.transform.position = base.transform.position;
		this.dustObject.SetActive(true);
		this.dustAnimator.SetTrigger("dust");
		this.movementMode = 2;
		this.animator.SetTrigger("isDashin");
		this.playerHitbox.enabled = false;
		this.playerHitbox.enabled = true;
		yield return new WaitForSeconds(0.1f);
		if (this.superDash != 0)
		{
			this.superDash = dashType;
			base.StartCoroutine(this.MaterialSwapper());
			this.moveSpeed = 12f;
			this.playerHitbox.radius = 0.1f;
		}
		this.movementMode = -2;
		if (this.superDash != 0)
			yield return new WaitForSeconds(0.1f);
		this.movementMode = -1;
		this.dashing = false;
		yield return new WaitForSeconds(0.25f);
		if (this.superDash == 0)
		{
			yield return new WaitForSeconds(0.1f);
		}
		else
		{
			this.superDash = 1;
		}
		this.movementMode = 0;
		yield break;
	}

	// Token: 0x060002BF RID: 703
	public IEnumerator MaterialSwapper()
	{
		if (this.bodyparts[0].material != this.whiteMaterial)
		{
			for (int i = 0; i < this.bodyparts.Length; i++)
			{
				this.bodyparts[i].material = this.whiteMaterial;
			}
			yield return new WaitForSeconds(0.1f);
			for (int j = 0; j < this.bodyparts.Length; j++)
			{
				this.bodyparts[j].material = this.spriteMaterial;
			}
		}
		else
		{
			yield return new WaitForSeconds(0.1f);
		}
		yield break;
	}

	// Token: 0x060002C0 RID: 704
	public void InputConfirm()
	{
		if (this.target != null && !this.inputsBlocked)
		{
			this.targetScript.Interaction();
		}
	}

	// Token: 0x060002C1 RID: 705
	public void TargetUpdate()
	{
		this.target = Physics2D.OverlapCircle(this.rb.position, 1f, this.mask);
		if (this.target != this.oldTarget)
		{
			if (this.oldTarget != null)
			{
				this.targetScript.Highlight(false);
			}
			if (this.target != null)
			{
				this.targetScript = this.target.transform.GetComponent<IHighlightable>();
				this.targetScript.Highlight(true);
			}
			this.oldTarget = this.target;
		}
	}

	// Token: 0x060002C2 RID: 706
	private void FixedUpdate()
	{
		float num = Vector2.Distance(this.rb.position, this.droneRb.position);
		if ((double)num > 0.3 && this.rotationScript.droneState != 1)
		{
			Vector2 normalized = (this.rb.position - this.droneRb.position).normalized;
			this.droneRb.velocity = normalized * this.moveSpeed * num;
			if (normalized.x >= 0.01f && this.droneTransform.localScale.x == 1f)
			{
				this.droneTransform.localScale = new Vector3(-1f, 1f, 1f);
			}
			else if (normalized.x <= -0.01f && this.droneTransform.localScale.x == -1f)
			{
				this.droneTransform.localScale = new Vector3(1f, 1f, 1f);
			}
		}
		else
		{
			this.droneRb.velocity = Vector2.zero;
		}
		if (this.movementMode == 2)
		{
			this.rb.velocity = (this.dashDir + this.moveDir).normalized * (this.moveSpeed * 3f);
		}
		else if (this.movementMode == -2)
		{
			this.rb.velocity = this.dashDir * 2f;
		}
		else
		{
			if (this.moveDir == Vector2.zero)
			{
				this.animator.SetBool("runnin", false);
				return;
			}
			this.rb.velocity = this.moveDir * this.moveSpeed;
			if (this.moveDir.x >= 0.01f && this.array[0] == 0)
			{
				this.Turnaround(-1f);
			}
			else if (this.moveDir.x <= -0.01f && this.array[0] == 1)
			{
				this.Turnaround(1f);
			}
		}
		if (!this.gameBegan && this.rotationScript.dashCount == 5)
		{
			this.gameBegan = true;
			this.rotationScript.GameStarter();
		}
		this.animator.SetBool("runnin", true);
		this.EatEnder();
	}

	// Token: 0x060002C3 RID: 707
	public void FootstepsAudio()
	{
		ManagerScript.instance.PlayShortSound(this.footstep[UnityEngine.Random.Range(0, this.footstep.Length)], 0);
	}

	// Token: 0x060002C4 RID: 708
	public void Turnaround(float scale)
	{
		this.spriteTransform.localScale = new Vector3(scale, 1f, 1f);
		this.ArraySwapper();
		this.handSprite[this.array[0]].sprite = this.handspriteArray[this.array[0]][this.hand[0]];
		this.handSprite[this.array[1]].sprite = this.handspriteArray[this.array[1]][this.hand[1]];
	}

	// Token: 0x060002C5 RID: 709
	public void ArraySwapper()
	{
		int num = this.array[0];
		this.array[0] = this.array[1];
		this.array[1] = num;
	}

	// Token: 0x060002C6 RID: 710
	public void JunkPickup(int food)
	{
		this.hand[this.oldSlot] = food;
		this.handSprite[this.array[this.oldSlot]].sprite = this.handspriteArray[this.array[this.oldSlot]][food];
		this.oldSlot++;
		if (this.oldSlot > 1)
		{
			this.oldSlot = 0;
		}
		base.StartCoroutine(this.MaterialSwapper());
	}

	// Token: 0x060002C7 RID: 711
	public void JunkDeposit(int slot)
	{
		this.hand[slot] = 0;
		this.handSprite[this.array[slot]].sprite = this.handspriteArray[this.array[slot]][0];
		this.oldSlot = slot;
	}

	// Token: 0x060002C8 RID: 712
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (collision.tag == "Deadly")
		{
			this.Gameover();
		}
	}

	// Token: 0x060002C9 RID: 713
	public void Gameover()
	{
		this.rotationScript.Gameover(base.transform, true, 1);
	}

	// Token: 0x040002C0 RID: 704
	public Transform spriteTransform;

	// Token: 0x040002C1 RID: 705
	public Sprite[] junkSpriteArray;

	// Token: 0x040002C2 RID: 706
	public Sprite[] backhandSpriteArray;

	// Token: 0x040002C3 RID: 707
	public Sprite[] fronthandSpriteArray;

	// Token: 0x040002C4 RID: 708
	public Sprite[][] handspriteArray;

	// Token: 0x040002C5 RID: 709
	public int[] hand;

	// Token: 0x040002C6 RID: 710
	public SpriteRenderer[] handSprite;

	// Token: 0x040002C7 RID: 711
	public bool inputsBlocked = true;

	// Token: 0x040002C8 RID: 712
	public LayerMask mask;

	// Token: 0x040002C9 RID: 713
	public RotationScript rotationScript;

	// Token: 0x040002CA RID: 714
	public SpriteRenderer[] bodyparts;

	// Token: 0x040002CB RID: 715
	public Material whiteMaterial;

	// Token: 0x040002CC RID: 716
	public bool dashing;

	// Token: 0x040002CD RID: 717
	public int superDash;

	// Token: 0x040002CE RID: 718
	private Rigidbody2D rb;

	// Token: 0x040002CF RID: 719
	private Animator animator;

	// Token: 0x040002D0 RID: 720
	private Material spriteMaterial;

	// Token: 0x040002D1 RID: 721
	public Animator dustAnimator;

	// Token: 0x040002D2 RID: 722
	public GameObject dustObject;

	// Token: 0x040002D3 RID: 723
	public ShockwaveScript[] shock;

	// Token: 0x040002D4 RID: 724
	public Vector2 dashDir = new Vector2(-1f, 0f);

	// Token: 0x040002D5 RID: 725
	public Rigidbody2D droneRb;

	// Token: 0x040002D6 RID: 726
	public Transform droneTransform;

	// Token: 0x040002D7 RID: 727
	public Animator droneHolo;

	// Token: 0x040002D8 RID: 728
	public AudioClip[] footstep;

	// Token: 0x040002D9 RID: 729
	public AudioClip[] superdashClips;

	// Token: 0x040002DA RID: 730
	public AudioClip[] tutorialClips;

	// Token: 0x040002DB RID: 731
	public AudioClip eatClip;

	// Token: 0x040002DC RID: 732
	public AudioClip[] dashClips;

	// Token: 0x040002DD RID: 733
	private AudioSource eatSource;

	// Token: 0x040002DE RID: 734
	public float moveSpeed = 8f;

	// Token: 0x040002DF RID: 735
	public float chargeEase = 0.5f;

	// Token: 0x040002E0 RID: 736
	public bool moveDrag;

	// Token: 0x040002E1 RID: 737
	public bool inMenu;

	// Token: 0x040002E2 RID: 738
	public bool inCutscene;

	// Token: 0x040002E3 RID: 739
	public int movementMode;

	// Token: 0x040002E4 RID: 740
	private Vector2 moveDir;

	// Token: 0x040002E5 RID: 741
	private Vector2 moveDirNew;

	// Token: 0x040002E6 RID: 742
	private Collider2D target;

	// Token: 0x040002E7 RID: 743
	private Collider2D oldTarget;

	// Token: 0x040002E8 RID: 744
	private IHighlightable targetScript;

	// Token: 0x040002E9 RID: 745
	private int oldSlot;

	// Token: 0x040002EA RID: 746
	private int[] array;

	// Token: 0x040002EB RID: 747
	private bool gameBegan;

	// Token: 0x040002EC RID: 748
	private Vector2 secondaryDashDir = new Vector2(-1f, 0f);

	// Token: 0x040002ED RID: 749
	private Coroutine currentDash;

	// Token: 0x040002EE RID: 750
	private CircleCollider2D playerHitbox;

	// Token: 0x040002EF RID: 751
	private int babeczkiState;

	// Token: 0x040002F0 RID: 752
	private Coroutine babeczkiRoutine;

	// Token: 0x040002F1 RID: 753
	private int babeczkiCount;
}
