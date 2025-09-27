using System;
using UnityEngine;

// Token: 0x02000027 RID: 39
public class ThrowableScript : ProjectileScript
{
    // Token: 0x060000B5 RID: 181 RVA: 0x000050A4 File Offset: 0x000032A4
    public void Throw(Vector3 origin, Vector2 dir)
	{
		base.transform.position = origin;
		if (this.trail != null)
		{
			this.trail.Clear();
		}
		float num = Mathf.Atan2(dir.y, dir.x) * 57.29578f;
		if (num < 0f)
		{
			num += 360f;
		}
		this.spriteRotator.eulerAngles = new Vector3(0f, 0f, num);
		base.gameObject.SetActive(true);
		this.animator.SetTrigger("fire");
		this.rb.AddForce(dir * this.speed, ForceMode2D.Impulse);
		if (this.gameObject.tag == "Deadly")
			AwariaInterface.MainInterface.Instance.addHazard(this);
		else
			Debug.Log("Not deadly");
	}
    public override void Destruction()
    {
        base.Destruction();
    }

	// Token: 0x04000086 RID: 134
	public Transform spriteRotator;
}
