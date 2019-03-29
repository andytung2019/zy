
public class ModCmd {
	private int cmd_id;
	private int device_id;
	private int control_type_id;
	private int control_code ;

	public ModCmd() {
		
		
	}
	
	public int getId() {
		return cmd_id;
	}
	public int getDevice(){
		return device_id;
	}
	public int getCmd(){
		return control_type_id;
	}
	public int getControlCode(){
		return control_code;
	}
	
	public void setId(int id) {
		cmd_id = id;
	}
	
	public void setDevice(int id) {
		device_id = id;
	}
	public void SetControlType(int md) {
		control_type_id = md;
	}
	public void setControlCode(int pm) {
		control_code = pm;
	}
}
