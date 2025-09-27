using System;
using System.Collections;
using UnityEngine;

// Token: 0x02000021 RID: 33
public class ShockwaveScript : MonoBehaviour
{
    // Token: 0x0600008E RID: 142 RVA: 0x000049EC File Offset: 0x00002BEC
    public void Emit(Vector3 origin, bool advanced, Vector2 target)
	{
		base.transform.position = origin;
		base.gameObject.SetActive(true);
		base.StartCoroutine(this.Discharging());
        if (this.gameObject.tag == "Deadly")
            AwariaInterface.MainInterface.Instance.addHazard(this);
        if (advanced)
		{
			this.rb.AddForce(target * 700f);
			return;
		}
		this.rb.AddForce(this.dir.normalized * this.speed, ForceMode2D.Impulse);
	}

	// Token: 0x0600008F RID: 143 RVA: 0x00002649 File Offset: 0x00000849
	public IEnumerator Discharging()
	{
		int number = 0;
		int num2;
		for (int i = 0; i < this.electroVfx.Length; i = num2 + 1)
		{
			yield return new WaitForSeconds(0.05f);
			int num = UnityEngine.Random.Range(1, 8);
			if (num == number)
			{
				number = UnityEngine.Random.Range(1, 8);
			}
			else
			{
				number = num;
			}
			this.electroVfx[i].Discharge(base.transform.position, number.ToString());
			num2 = i;
		}
		this.Destruction();
		yield break;
	}

	// Token: 0x06000090 RID: 144 RVA: 0x00002658 File Offset: 0x00000858
	public virtual void OnCollisionEnter2D()
	{
		this.Destruction();
	}

	// Token: 0x06000091 RID: 145 RVA: 0x00002660 File Offset: 0x00000860
	// Added the remove to both the destruction and onDestroy to ensure it goes off
	public void Destruction()
	{
		if (base.gameObject.activeSelf)
		{
			base.StopAllCoroutines();
			this.rb.velocity = Vector2.zero;
			base.gameObject.SetActive(false);
		}
	}

	// Token: 0x04000062 RID: 98
	public float speed = 8f;

	// Token: 0x04000063 RID: 99
	public ElectroVfxScript[] electroVfx;

	// Token: 0x04000064 RID: 100
	public Vector2 dir;

	// Token: 0x04000065 RID: 101
	public Rigidbody2D rb;
}
