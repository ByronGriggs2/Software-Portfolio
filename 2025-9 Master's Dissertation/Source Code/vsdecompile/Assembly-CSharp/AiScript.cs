using System;
using System.Collections;
using Pathfinding;
using UnityEngine;

// Token: 0x02000035 RID: 53
public abstract class AiScript : MonoBehaviour
{
	// Token: 0x060000EF RID: 239 RVA: 0x00005904 File Offset: 0x00003B04
	protected virtual void Start()
	{
		this.seeker = base.GetComponent<Seeker>();
		this.rb = base.GetComponent<Rigidbody2D>();
		this.animator = base.GetComponent<Animator>();
		this.deathCollider = base.GetComponent<CircleCollider2D>();
		base.InvokeRepeating("UpdatePath", 0f, 0.2f);
		base.gameObject.SetActive(false);
	}

	// Token: 0x060000F0 RID: 240 RVA: 0x00005964 File Offset: 0x00003B64
	private void UpdatePath()
	{
		if (this.seeker.IsDone())
		{
			this.seeker.StartPath(this.rb.position, this.playerTransform.position, new OnPathDelegate(this.OnPathComplete));
		}
	}

	// Token: 0x060000F1 RID: 241 RVA: 0x000029F2 File Offset: 0x00000BF2
	private void OnPathComplete(Path p)
	{
		if (!p.error)
		{
			this.path = p;
			this.currentWaypoint = 0;
		}
	}

	// Token: 0x060000F2 RID: 242 RVA: 0x00002A0A File Offset: 0x00000C0A
	protected virtual IEnumerator ChaseRoutine()
	{
		this.playerInRange();
		yield return new WaitForSeconds(0.2f);
		yield break;
	}

	// Token: 0x060000F3 RID: 243 RVA: 0x000059B4 File Offset: 0x00003BB4
	protected bool playerInRange()
	{
		if ((base.transform.position - this.playerTransform.position).magnitude > 0.5f)
		{
			RaycastHit2D hit = Physics2D.CircleCast(base.transform.position, this.attackWidth, this.playerTransform.position - base.transform.position, this.attackRange, this.dashMask);
			if (!hit || hit.collider.tag != "Player")
			{
				return false;
			}
		}
		this.PreAction();
		this.InRangeAction();
		return true;
	}

	// Token: 0x060000F4 RID: 244 RVA: 0x00005A68 File Offset: 0x00003C68
	protected void PreAction()
	{
		this.busy = true;
		this.rb.velocity = Vector2.zero;
		this.aim = this.playerTransform.position;
		if (base.transform.position.x < this.aim.x)
		{
			this.spriteTransform.localScale = new Vector3(-1f, 1f, 1f);
			return;
		}
		if (base.transform.position.x > this.aim.x)
		{
			this.spriteTransform.localScale = new Vector3(1f, 1f, 1f);
		}
	}

	// Token: 0x060000F5 RID: 245 RVA: 0x000020D2 File Offset: 0x000002D2
	protected virtual void InRangeAction()
	{
	}

	// Token: 0x060000F6 RID: 246 RVA: 0x00002A19 File Offset: 0x00000C19
	protected virtual IEnumerator PostAction(float actionLenght = 1.3f)
	{
		yield return new WaitForSeconds(actionLenght);
		this.busy = false;
		this.hitable = true;
		if (!this.playerInRange())
		{
			Vector2 temp;
			temp.x = this.path.vectorPath[this.currentWaypoint].x;
			temp.y = this.path.vectorPath[this.currentWaypoint].y;
			Vector2 normalized = (temp - this.rb.position).normalized;
			if (Vector2.Distance(this.rb.position, this.path.vectorPath[this.currentWaypoint]) < this.nextWaypointDistance)
			{
				this.currentWaypoint++;
			}
			temp.x = this.path.vectorPath[this.currentWaypoint].x;
			temp.y = this.path.vectorPath[this.currentWaypoint].y;
			normalized = (temp - this.rb.position).normalized;
			if (normalized.x >= 0.1f)
			{
				this.spriteTransform.localScale = new Vector3(-1f, 1f, 1f);
			}
			else if (normalized.x <= -0.1f)
			{
				this.spriteTransform.localScale = new Vector3(1f, 1f, 1f);
			}
			this.animator.SetTrigger("isMoving");
			base.StartCoroutine(this.ChaseRoutine());
		}
		yield break;
	}

	// Token: 0x060000F7 RID: 247 RVA: 0x00005B1C File Offset: 0x00003D1C
	protected virtual void FixedUpdate()
	{
		if (this.busy || this.path == null || this.currentWaypoint >= this.path.vectorPath.Count)
		{
			return;
		}
		Vector2 temp;
		temp.x = this.path.vectorPath[this.currentWaypoint].x;
		temp.y = this.path.vectorPath[this.currentWaypoint].y;
		Vector2 normalized = (temp - this.rb.position).normalized;
		this.rb.AddForce(normalized * this.moveSpeed);
		if (Vector2.Distance(this.rb.position, this.path.vectorPath[this.currentWaypoint]) < this.nextWaypointDistance)
		{
			this.currentWaypoint++;
		}
		if (this.rb.velocity.x >= this.turnSensitivity)
		{
			this.spriteTransform.localScale = new Vector3(-1f, 1f, 1f);
			return;
		}
		if (this.rb.velocity.x <= -this.turnSensitivity)
		{
			this.spriteTransform.localScale = new Vector3(1f, 1f, 1f);
		}
	}

	// Token: 0x060000F8 RID: 248 RVA: 0x00002A2F File Offset: 0x00000C2F
	public virtual void Spawn()
	{
		base.gameObject.SetActive(true);
		this.busy = true;
		base.StartCoroutine(this.PostAction(this.spawnTime));
		this.animator.SetTrigger("isAlive");
	}

	// Token: 0x060000F9 RID: 249 RVA: 0x00002A67 File Offset: 0x00000C67
	public void SpawnAudio()
	{
		if (this.alreadySpawned)
		{
			return;
		}
		ManagerScript.instance.PlayShortSound(this.spawnClip, 0);
		this.alreadySpawned = true;
	}

	// Token: 0x040000C0 RID: 192
	private Seeker seeker;

	// Token: 0x040000C1 RID: 193
	protected Rigidbody2D rb;

	// Token: 0x040000C2 RID: 194
	protected Animator animator;

	// Token: 0x040000C3 RID: 195
	public Transform playerTransform;

	// Token: 0x040000C4 RID: 196
	protected CircleCollider2D deathCollider;

	// Token: 0x040000C5 RID: 197
	public LayerMask playerMask;

	// Token: 0x040000C6 RID: 198
	public LayerMask dashMask;

	// Token: 0x040000C7 RID: 199
	public Transform spriteTransform;

	// Token: 0x040000C8 RID: 200
	public AudioClip spawnClip;

	// Token: 0x040000C9 RID: 201
	public float moveSpeed = 40f;

	// Token: 0x040000CA RID: 202
	public float dashPower = 1500f;

	// Token: 0x040000CB RID: 203
	public float nextWaypointDistance = 0.5f;

	// Token: 0x040000CC RID: 204
	public float attackRange = 2.8f;

	// Token: 0x040000CD RID: 205
	public float attackWidth = 0.3f;

	// Token: 0x040000CE RID: 206
	public float spawnTime;

	// Token: 0x040000CF RID: 207
	private Path path;

	// Token: 0x040000D0 RID: 208
	private int currentWaypoint;

	// Token: 0x040000D1 RID: 209
	protected Vector2 aim;

	// Token: 0x040000D2 RID: 210
	protected bool busy;

	// Token: 0x040000D3 RID: 211
	protected bool hitable;

	// Token: 0x040000D4 RID: 212
	private float turnSensitivity = 0.4f;

	// Token: 0x040000D5 RID: 213
	private bool alreadySpawned;
}
