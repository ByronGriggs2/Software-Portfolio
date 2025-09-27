using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Token: 0x02000005 RID: 5
public class ArmSummonScript : MonoBehaviour
{
	// Token: 0x0600000F RID: 15 RVA: 0x000020E6 File Offset: 0x000002E6
	private void Start()
	{
		base.StartCoroutine(this.ArmSpawn());
	}

	// Token: 0x06000010 RID: 16 RVA: 0x000020F5 File Offset: 0x000002F5
	private IEnumerator ArmSpawn()
	{
		yield return new WaitForSeconds(0.5f);
		if (this.rotationScript.soloHaunt)
		{
			this.arms.Add(UnityEngine.Object.Instantiate<GameObject>(this.arm, base.transform.position, Quaternion.identity));
			yield return new WaitForSeconds(0.5f);
		}
		base.StartCoroutine(this.ArmSpawn());
		yield break;
	}

	// Token: 0x06000011 RID: 17 RVA: 0x00003CE4 File Offset: 0x00001EE4
	public void WipeEvent()
	{
		for (int i = 0; i < this.arms.Count; i++)
		{
			UnityEngine.Object.Destroy(this.arms[i]);
		}
		base.StopAllCoroutines();
	}

	// Token: 0x04000007 RID: 7
	public RotationScript rotationScript;

	// Token: 0x04000008 RID: 8
	public GameObject arm;

	// Token: 0x04000009 RID: 9
	private List<GameObject> arms = new List<GameObject>();
}
