using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200001B RID: 27
public class PhaseScript : MonoBehaviour
{
	// Token: 0x06000071 RID: 113 RVA: 0x000024A5 File Offset: 0x000006A5
	public void Phase(Vector3 target, Vector3 origin, Vector3 localScale)
	{
		base.transform.position = origin;
		this.spriteTransform.localScale = localScale;
		base.gameObject.SetActive(true);
		base.StartCoroutine(this.MoveToPosition(target));
	}

	// Token: 0x06000072 RID: 114 RVA: 0x000024D9 File Offset: 0x000006D9
	public IEnumerator MoveToPosition(Vector3 target)
	{
		Vector3 currentPos = base.transform.position;
		float t = 0f;
		while (t < 1f)
		{
			t += Time.deltaTime / this.timeToMove;
			this.phaseSprite.color = new Color(1f, 1f, 1f, 1f - t);
			base.transform.position = Vector3.Lerp(currentPos, target, t);
			yield return null;
		}
		base.gameObject.SetActive(false);
		yield break;
	}

	// Token: 0x04000041 RID: 65
	public SpriteRenderer phaseSprite;

	// Token: 0x04000042 RID: 66
	public Transform spriteTransform;

	// Token: 0x04000043 RID: 67
	private float timeToMove = 0.2f;
}
