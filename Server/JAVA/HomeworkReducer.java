
/*
 * HomeworkReducer.java
 *
 * Created on Oct 22, 2012, 5:46:32 PM
 */

package org.smaple;


import java.io.IOException;
// import org.apache.commons.logging.Log;
// import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.io.Text;

/**
 *
 * @author mac
 */
public class HomeworkReducer extends Reducer<Text,Text,Text,Text> {
    // The Karmasphere Studio Workflow Log displays logging from Apache Commons Logging, for example:
    // private static final Log LOG = LogFactory.getLog("org.smaple.HomeworkReducer");

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context)
                        throws IOException, InterruptedException {
    	// TODO: please implement your reducer here
    	String[] Keylinks = key.toString().split(" , ");  	
    	String link1 = Keylinks[0];
    	String link2 = Keylinks[1];
    	
    	String Vallinks = " ";
    	for(Text val : values)
    	{		
    		String value = val.toString();
    		if(!value.equals(link1)&& !value.equals(link2))
    		{   			
    				Vallinks = value;
    				context.write(key,new Text(Vallinks));
    		}
    	}
    }
}
