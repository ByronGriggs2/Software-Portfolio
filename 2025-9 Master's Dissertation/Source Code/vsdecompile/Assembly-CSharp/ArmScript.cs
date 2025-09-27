using System;
using UnityEngine;

// Token: 0x02000004 RID: 4
public class ArmScript : MonoBehaviour
{
	// Token: 0x0600000D RID: 13 RVA: 0x00003C80 File Offset: 0x00001E80
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (collision.tag == "Player")
		{
			PlayerScript component = collision.GetComponent<PlayerScript>();
			if (component.superDash == 2 && !component.inputsBlocked)
			{
				UnityEngine.Object.Instantiate<GameObject>(this.deathVfx, base.transform.position, base.transform.rotation);
				UnityEngine.Object.Destroy(base.gameObject);
			}
		}
	}

	// Token: 0x04000006 RID: 6
	public GameObject deathVfx;
}
