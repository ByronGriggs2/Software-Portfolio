using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200002C RID: 44
public class AudioSourceRemover : MonoBehaviour
{
	// Token: 0x060000CA RID: 202 RVA: 0x00002856 File Offset: 0x00000A56
	private void Start()
	{
		this.source = base.GetComponent<AudioSource>();
		base.StartCoroutine(this.Countdown());
	}

	// Token: 0x060000CB RID: 203 RVA: 0x00002871 File Offset: 0x00000A71
	private IEnumerator Countdown()
	{
		yield return new WaitForSeconds(0.1f);
		yield return new WaitForSeconds(this.source.clip.length);
		ManagerScript.instance.AudioRemover(this.source);
		UnityEngine.Object.Destroy(base.gameObject);
		yield break;
	}

	// Token: 0x04000097 RID: 151
	private AudioSource source;
}
