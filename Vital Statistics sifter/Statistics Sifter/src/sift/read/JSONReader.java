package sift.read;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public abstract class  JSONReader {

	/**
	 * 	The original data source to be read
	 */
	protected JSONObject jsonObj = null;

	/**
	 * ctor
	 * @param fileName
	 * @throws FileNotFoundException
	 * @throws IOException
	 * @throws ParseException
	 */
	public JSONReader(String fileName) throws FileNotFoundException, IOException, ParseException {
		JSONParser parser = new JSONParser();
		Object obj = parser.parse(new FileReader(fileName));
		this.jsonObj = (JSONObject) obj;	
	}
}
