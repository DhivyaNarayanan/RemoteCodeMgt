
/*
 * HomeworkMapper.java
 *
 * Created on Oct 22, 2012, 5:41:50 PM
 */

package org.smaple;


import java.io.IOException;

// import org.apache.commons.logging.Log;
// import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.mapreduce.Mapper;

import org.apache.hadoop.io.Text;
import java.util.Vector;

/**
 *
 * @author mac
 */
public class HomeworkMapper extends Mapper<Text,Text,Text,Text> {
    // The Karmasphere Studio Workflow Log displays logging from Apache Commons Logging, for example:
    // private static final Log LOG = LogFactory.getLog("org.smaple.HomeworkMapper");
	Vector<String> linkfromVec = new Vector<String>();
	Vector<String> linktoVec = new Vector<String>();
    @Override
    protected void map(Text key, Text value, Context context)
                    throws IOException, InterruptedException {
    	// TODO: please implement your mapper code here
    	
    	
    	linkfromVec.add(key.toString());
    	linktoVec.add(value.toString());
    	
    	if(linktoVec.size()==8)
    	{
    		for(int i=0; i<linktoVec.size();i++)		
        	{
        		String tmplinkto = linktoVec.get(i);
        		for(int j=0; j<linkfromVec.size();j++)
        		{
        			String tfrom = linkfromVec.get(j);
        			if(tfrom.equals(tmplinkto))
        			{
        				String Keylink = linktoVec.get(j);
        				Keylink += " , ";
        				Keylink += tfrom;
        				String Valuelink = linkfromVec.get(i);
        				context.write(new Text(Keylink),new Text(Valuelink));   				
        			}
        		}    		
        	}
    	}
    	
    }
}
