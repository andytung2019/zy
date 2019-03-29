import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletRequest;

import java.awt.List;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

import net.sf.json.JSONObject; 

public class SendServlet extends HttpServlet{
	protected void service(HttpServletRequest request, HttpServletResponse response) 
            throws ServletException, IOException {
		
		//cmd : ÉèÖÃÎÂ¶È 2, 55.5 ->555
		//ModCmd cmd = new ModCmd(); 
		//cmd.setId(10001);
		//cmd.setDevice(201);
		//cmd.SetControlType(2);
		//cmd.setControlCode(555);
		
		Map map = new HashMap();
		map.put("cmd_id", 10001);
		map.put("device_id", 3501);
		map.put("contorl_type_id", 2);
		map.put("contorl_code", 555);
		
		
        JSONObject jsn =  new JSONObject().fromObject(map);
        //System.out.println("new jsn");
        
        //jsn.put("cmd", JSONObject.fromObject(cmd));
        response.setContentType("text/html;charset=utf-8"); 
        response.getWriter().print(jsn);
	}
}
