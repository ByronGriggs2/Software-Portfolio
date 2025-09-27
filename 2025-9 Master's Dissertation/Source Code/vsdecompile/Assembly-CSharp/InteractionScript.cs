using System;
using UnityEngine;

// Token: 0x02000060 RID: 96
public class InteractionScript : MonoBehaviour
{
	// Token: 0x0600020B RID: 523 RVA: 0x00008E30 File Offset: 0x00007030
	private void Update()
	{
        if (AwariaInterface.MainInterface.Instance.getPrimary())
		{
			if (this.player.inMenu)
			{
				this.menu.InputConfirm();
				return;
			}
			if (this.player.inCutscene)
			{
				this.cutscene.InputConfirm();
				return;
			}
			if (this.upscaleAniScript.inIntro)
			{
				this.upscaleAniScript.Activator("introEnd", base.transform, 1);
				return;
			}
			this.player.InputConfirm();
		}
	}

	// Token: 0x040001C2 RID: 450
	public PlayerScript player;

	// Token: 0x040001C3 RID: 451
	public MenuScript menu;

	// Token: 0x040001C4 RID: 452
	public CutsceneScript cutscene;

	// Token: 0x040001C5 RID: 453
	public UpscaleAnimatorScript upscaleAniScript;
}
