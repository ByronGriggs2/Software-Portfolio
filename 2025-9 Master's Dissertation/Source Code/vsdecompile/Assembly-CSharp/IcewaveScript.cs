using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000017 RID: 23
public class IcewaveScript : MonoBehaviour
{
	// Token: 0x06000061 RID: 97 RVA: 0x000023A7 File Offset: 0x000005A7
	public void Throw(Vector3 origin, Vector2 dir)
	{
		base.transform.position = origin;
		base.gameObject.SetActive(true);
		base.StartCoroutine(this.Discharging());
		this.rb.AddForce(dir * this.speed, ForceMode2D.Impulse);
	}

	// Token: 0x06000062 RID: 98 RVA: 0x000023E6 File Offset: 0x000005E6
	public IEnumerator Discharging()
	{
		yield return new WaitForSeconds(this.wait);
		int num;
		for (int i = 0; i < 5; i = num + 1)
		{
			yield return new WaitForSeconds((float)UnityEngine.Random.Range(5, 12) * 0.01f);
			this.spikePool.SpawnIce(base.transform.position);
			num = i;
		}
		this.Destruction();
		yield break;
	}

	// Token: 0x06000063 RID: 99 RVA: 0x000023F5 File Offset: 0x000005F5
	public virtual void OnCollisionEnter2D()
	{
		this.Destruction();
	}

	// Token: 0x06000064 RID: 100 RVA: 0x000023FD File Offset: 0x000005FD
	public void Destruction()
	{
		if (base.gameObject.activeSelf)
		{
			base.StopAllCoroutines();
			this.rb.velocity = Vector2.zero;
			base.gameObject.SetActive(false);
		}
	}

	// Token: 0x04000038 RID: 56
	public float speed = 8f;

	// Token: 0x04000039 RID: 57
	public StrigaAiScript spikePool;

	// Token: 0x0400003A RID: 58
	public Rigidbody2D rb;

	// Token: 0x0400003B RID: 59
	public float wait;
}
