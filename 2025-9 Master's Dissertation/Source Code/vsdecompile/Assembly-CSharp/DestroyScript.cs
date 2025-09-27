using System;
using UnityEngine;

// Token: 0x02000032 RID: 50
public class DestroyScript : MonoBehaviour
{
	// Token: 0x060000E7 RID: 231 RVA: 0x0000295A File Offset: 0x00000B5A
	private void Start()
	{
		UnityEngine.Object.Destroy(base.gameObject, 1f);
	}
}
