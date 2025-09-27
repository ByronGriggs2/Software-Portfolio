using System;
using UnityEngine;

// Token: 0x0200000B RID: 11
public class BulletSpreadPoolScript : MonoBehaviour
{
	// Token: 0x06000032 RID: 50 RVA: 0x00002243 File Offset: 0x00000443
	private void Start()
	{
		this.allSpreads = base.GetComponentsInChildren<BulletSpreadScript>();
	}

	// Token: 0x06000033 RID: 51 RVA: 0x00002251 File Offset: 0x00000451
	public void Ignite(Vector3 origin)
	{
		if (this.spreadIndex == this.allSpreads.Length)
		{
			this.spreadIndex = 0;
		}
		this.allSpreads[this.spreadIndex].Spread(origin);
		this.spreadIndex++;
	}

	// Token: 0x06000034 RID: 52 RVA: 0x000040C0 File Offset: 0x000022C0
	public void WipeEvent()
	{
		for (int i = 0; i < this.allSpreads.Length; i++)
		{
			this.allSpreads[i].Wipe();
		}
	}

	// Token: 0x0400001F RID: 31
	public BulletSpreadScript[] allSpreads;

	// Token: 0x04000020 RID: 32
	public int spreadIndex;
}
