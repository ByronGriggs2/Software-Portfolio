using System;
using System.Collections;
using UnityEngine;

// Token: 0x0200002F RID: 47
public class CameraScript : MonoBehaviour
{
	// Token: 0x060000D5 RID: 213 RVA: 0x00002897 File Offset: 0x00000A97
	private void Awake()
	{
		Application.targetFrameRate = 60;
	}

	// Token: 0x060000D6 RID: 214 RVA: 0x000028A0 File Offset: 0x00000AA0
	private void Start()
	{
		ManagerScript.instance.MusicCheck(this.music);
		this.CamRescale();
	}

	// Token: 0x060000D7 RID: 215 RVA: 0x000028B8 File Offset: 0x00000AB8
	private void Update()
	{
		this.CamRescale();
	}

	// Token: 0x060000D8 RID: 216 RVA: 0x000053DC File Offset: 0x000035DC
	public void Shakedown(float shakeDuration, float shakeAmount = 0.1f)
	{
		if (this.shakeActive)
		{
			base.StopCoroutine(this.currentShake);
			this.camTransform.localPosition = this.originalPos;
		}
		this.originalPos = this.camTransform.localPosition;
		this.currentShake = base.StartCoroutine(this.Shake(shakeDuration, shakeAmount));
	}

	// Token: 0x060000D9 RID: 217 RVA: 0x000028C0 File Offset: 0x00000AC0
	public IEnumerator Shake(float shakeDuration, float shakeAmount)
	{
		this.shakeActive = true;
		float shakeDurationInitial = shakeDuration;
		while (shakeDuration > 0f)
		{
			this.camTransform.localPosition = this.originalPos + UnityEngine.Random.insideUnitSphere * shakeAmount * Mathf.Clamp01(shakeDuration / shakeDurationInitial);
			shakeDuration -= Time.deltaTime;
			yield return new WaitForSeconds(Time.deltaTime);
		}
		this.camTransform.localPosition = this.originalPos;
		this.shakeActive = false;
		yield break;
	}

	// Token: 0x060000DA RID: 218 RVA: 0x00005434 File Offset: 0x00003634
	private void CamRescale()
	{
		if (Screen.width == this.ScreenSizeX && Screen.height == this.ScreenSizeY)
		{
			return;
		}
		float num = 1.7777778f;
		float num2 = (float)Screen.width / (float)Screen.height / num;
		if (num2 < 1f)
		{
			Rect rect = this.camera.rect;
			rect.width = 1f;
			rect.height = num2;
			rect.x = 0f;
			rect.y = (1f - num2) / 2f;
			this.camera.rect = rect;
		}
		else
		{
			float num3 = 1f / num2;
			Rect rect2 = this.camera.rect;
			rect2.width = num3;
			rect2.height = 1f;
			rect2.x = (1f - num3) / 2f;
			rect2.y = 0f;
			this.camera.rect = rect2;
		}
		this.ScreenSizeX = Screen.width;
		this.ScreenSizeY = Screen.height;
	}

	// Token: 0x0400009B RID: 155
	public Transform camTransform;

	// Token: 0x0400009C RID: 156
	public Camera camera;

	// Token: 0x0400009D RID: 157
	public AudioClip music;

	// Token: 0x0400009E RID: 158
	private Vector3 originalPos;

	// Token: 0x0400009F RID: 159
	private Coroutine currentShake;

	// Token: 0x040000A0 RID: 160
	private bool shakeActive;

	// Token: 0x040000A1 RID: 161
	private int ScreenSizeX;

	// Token: 0x040000A2 RID: 162
	private int ScreenSizeY;
}
