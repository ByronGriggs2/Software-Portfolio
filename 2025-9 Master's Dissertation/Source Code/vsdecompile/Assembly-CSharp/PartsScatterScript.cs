using System;
using UnityEngine;

// Token: 0x0200001A RID: 26
public class PartsScatterScript : MonoBehaviour
{
	// Token: 0x0600006E RID: 110 RVA: 0x000045EC File Offset: 0x000027EC
	private void Start()
	{
		base.GetComponent<Rigidbody2D>().AddForce(new Vector2(UnityEngine.Random.Range(-40f, 40f), UnityEngine.Random.Range(40f, 120f)));
		base.GetComponent<Rigidbody2D>().AddTorque(300f);
		UnityEngine.Object.Destroy(base.gameObject, UnityEngine.Random.Range(0.7f, 0.8f));
	}

	// Token: 0x0600006F RID: 111 RVA: 0x00002481 File Offset: 0x00000681
	private void Update()
	{
		if (base.transform.localPosition.y <= 0f)
		{
			UnityEngine.Object.Destroy(base.gameObject);
		}
	}
}
