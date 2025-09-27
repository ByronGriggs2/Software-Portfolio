using System;
using AwariaInterface;
using UnityEngine;
using UnityEngine.InputSystem;

// Token: 0x0200005F RID: 95
public class InputScript : MonoBehaviour
{
	// Token: 0x06000205 RID: 517 RVA: 0x00008D20 File Offset: 0x00006F20
	private void Awake()
	{
		if (InputScript.instance == null)
		{
			InputScript.instance = this;
		}
		this.playerInput = base.GetComponent<PlayerInput>();
		this.SetupActions();
		if (MainInterface.Instance == null)
		{
			new GameObject("ByronModLoader").AddComponent<MainInterface>();
		}
	}

	// Token: 0x06000206 RID: 518 RVA: 0x00003320 File Offset: 0x00001520
	private void Update()
	{
		this.UpdateInput();
	}

	// Token: 0x06000207 RID: 519 RVA: 0x00008D70 File Offset: 0x00006F70
	private void SetupActions()
	{
		this.primaryAction = this.playerInput.actions["Primary"];
		this.secondaryAction = this.playerInput.actions["Secondary"];
		this.cancelAction = this.playerInput.actions["Cancel"];
		this.moveAction = this.playerInput.actions["Move"];
	}

	// Token: 0x06000208 RID: 520 RVA: 0x00008DEC File Offset: 0x00006FEC
	private void UpdateInput()
	{
		MainInterface myInterface = MainInterface.Instance;
		this.moveInput = myInterface.getMoveVector();
		this.primaryKey = myInterface.getPrimary();
		this.secondaryKey = myInterface.getSecondary();
		this.cancelKey = myInterface.getCancel();
		//this.moveInput = this.moveAction.ReadValue<Vector2>();
		//this.primaryKey = this.primaryAction.WasPressedThisFrame();
		//this.secondaryKey = this.secondaryAction.WasPressedThisFrame();
		//this.cancelKey = this.cancelAction.WasPressedThisFrame();
	}

	// Token: 0x040001B8 RID: 440
	public static InputScript instance;

	// Token: 0x040001B9 RID: 441
	public Vector2 moveInput;

	// Token: 0x040001BA RID: 442
	public bool primaryKey;

	// Token: 0x040001BB RID: 443
	public bool secondaryKey;

	// Token: 0x040001BC RID: 444
	public bool cancelKey;

	// Token: 0x040001BD RID: 445
	private PlayerInput playerInput;

	// Token: 0x040001BE RID: 446
	private InputAction primaryAction;

	// Token: 0x040001BF RID: 447
	private InputAction secondaryAction;

	// Token: 0x040001C0 RID: 448
	private InputAction cancelAction;

	// Token: 0x040001C1 RID: 449
	private InputAction moveAction;
}
