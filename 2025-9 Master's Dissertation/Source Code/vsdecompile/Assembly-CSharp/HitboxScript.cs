using System;
using UnityEngine;

// Token: 0x0200004E RID: 78
public class HitboxScript : MonoBehaviour
{
	// Token: 0x0600019F RID: 415 RVA: 0x00007A20 File Offset: 0x00005C20
	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (collision.tag == "Player")
		{
			PlayerScript component = collision.GetComponent<PlayerScript>();
			if (component.superDash == 2 && !component.inputsBlocked)
			{
				this.target.Attacked(component.dashDir);
				return;
			}
		}
		else if (collision.tag == "veryDeadly")
		{
			this.target.Attacked(Vector2.zero);
		}
	}

	// Token: 0x04000166 RID: 358
	public IVulnerable target;
}
