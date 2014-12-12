import java.util.regex.Matcher;  
import java.util.regex.Pattern;  

public class RegexChk  
{  
  
    public  boolean startCheck(String reg,String ss)  
    {  
        boolean ret = false;  
          
        Pattern pattern = Pattern.compile(reg);  
        Matcher matcher=pattern.matcher(ss);  
          
        ret = matcher.matches();  
        return ret;  
    }  
      
  
     /** 
     *checking for interge, not with the head is  0 or -0 
     * */  
    public boolean checkNr(String interge)  
    {  
        String reg="^(-?)[1-9]+\\d*|0";  
        return startCheck(reg,nr);  
    }  
  
    /** 
     *checking for cellphone number, must be with the head is 1
     * */  
    public boolean checkCellPhone(String cellPhoneNr)  
    {  
        String reg="^[1][\\d]{10}";  
        return startCheck(reg,cellPhoneNr);  
    }  
      
    /** 
     *  checking for the username, with following rules:
     *  the range of username : a-,A-Z,0-9,"-",chinese without the "-" as the end
     *  the length of the username must between 4 and 20
     * */  
    public boolean checkUsername(String username,int min,int max)  
    {  
        String regex="[\\w\u4e00-\u9fa5]{"+min+","+max+"}(?<!_)";  
        return startCheck(regex,username);  
    }  
    /** 
     *  checking for the username, with following rules:
     *  the range of username : a-,A-Z,0-9,"-",chinese without the "-" as the end
     *  the length of the username must between 4 and 20
     * */  
    public boolean checkUsername(String username,int min)  
    {  
        //[\\w\u4e00-\u9fa5]{2,}?  
        String regex="[\\w\u4e00-\u9fa5]{"+min+",}(?<!_)";  
          
        return startCheck(regex,username);  
    } 
    /**
     *  checking for the ipv6 address, this regex can check any format ipv6 address
     */
    public boolean checkIpv6Address(String ipv6add){
       
       String regex = "^([\\da-fA-F]{1,4}:){6}((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}" + 
		    			
		    			"(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)|::([\\da.fA.F]1,4:)0,4((25[0.5]|2[0.4]\\d|[01]?\\d\\d?)\\.)3" + 
		    			
		    			"(25[0.5]|2[0.4]\\d|[01]?\\d\\d?)|^([\\da-fA-F]{1,4}:):([\\da-fA-F]{1,4}:){0,3}((25[0-5]|2[0-4]" +
		    			
		    			"\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)|([\\da.fA.F]1,4:)2:([\\da.fA.F]1,4:)0,2" + 
		    			
		    			"((25[0.5]|2[0.4]\\d|[01]?\\d\\d?)\\.)3(25[0.5]|2[0.4]\\d|[01]?\\d\\d?)|^([\\da-fA-F]{1,4}:){3}:" +
		    			
		    			"([\\da-fA-F]{1,4}:){0,1}((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)|" +
					
					"4:)4((:[\\da.fA.F]1,4)1,2|:)|^([\\da-fA-F]{1,4}:){5}:([\\da-fA-F]{1,4})?|([\\da.fA.F]1,4:)6:";		    			

       return startCheck(regex, ipv6add);
    } 
}
