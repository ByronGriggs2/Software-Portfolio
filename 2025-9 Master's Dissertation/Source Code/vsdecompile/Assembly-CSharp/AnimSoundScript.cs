using System;
using UnityEngine;

// Token: 0x0200006D RID: 109
public class AnimSoundScript : MonoBehaviour
{
	// Token: 0x0600026E RID: 622 RVA: 0x000035B9 File Offset: 0x000017B9
	public void AnimAudio(int number)
	{
		ManagerScript.instance.PlayLongSound(this.clipArray[number]);
	}

	// Token: 0x04000261 RID: 609
	public AudioClip[] clipArray;
}
