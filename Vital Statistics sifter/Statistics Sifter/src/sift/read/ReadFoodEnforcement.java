package sift.read;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.Scanner;

import org.json.simple.*;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class ReadFoodEnforcement extends JSONReader implements ReadData {
	
	
	/**
	 * The list of results for FDA data
	 */
	private ArrayList<JSONObject> results = null;

	/**
	 * ctor
	 * 
	 * @param fileName
	 * @throws FileNotFoundException
	 * @throws IOException
	 * @throws ParseException
	 */
	public ReadFoodEnforcement(String fileName) throws FileNotFoundException, IOException, ParseException {
		super(fileName);
		Collection<JSONObject> colection = ((Collection<JSONObject>) super.jsonObj.get("results"));
		this.results = new ArrayList<JSONObject>(colection);
	}

	/**
         * Generates new file
         * @throws Exception 
         */
	@Override
	public native void removeRepeats() throws Exception;

	@Override
	public native void readFile() throws Exception;

	/*
	 * 
	 * @Override public void removeRepeats() throws IOException, ParseException {
	 * 
	 * for (int i = 0; i < results.size() - 1; ++i) { JSONObject jObjPtr1 =
	 * results.get(i);
	 * 
	 * // add units and cases attribute putUnitsOrCase(jObjPtr1);
	 * 
	 * String recallInitDate1 = (String) jObjPtr1.get("recall_initiation_date");
	 * String recallingFirm1 = (String) jObjPtr1.get("recalling_firm"); for (int j =
	 * i + 1; j < results.size(); ++j) { JSONObject jObjPtr2 = results.get(j);
	 * String recallInitDate2 = (String) jObjPtr2.get("recall_initiation_date");
	 * String recallingFirm2 = (String) jObjPtr2.get("recalling_firm"); if
	 * (recallInitDate1 == recallInitDate2 && recallingFirm1 == recallingFirm2) {
	 * String productQuan = (String) jObjPtr2.get("product_quantity");
	 * 
	 * putUnitsOrCase(jObjPtr2); int units = Integer.parseInt((String)
	 * jObjPtr2.get("units")) + Integer.parseInt((String) jObjPtr1.get("units"));
	 * int cases = Integer.parseInt((String) jObjPtr2.get("cases")) +
	 * Integer.parseInt((String) jObjPtr1.get("cases"));
	 * 
	 * jObjPtr1.replace("units", Integer.toString(units)); jObjPtr1.replace("cases",
	 * Integer.toString(cases));
	 * 
	 * if (!results.remove(jObjPtr2)) { throw new
	 * RuntimeException("Error: did not remove repeat"); } --j; } } } }
	 * 
	 * private void putUnitsOrCase(JSONObject jObj) { String productQuan = (String)
	 * jObj.get("product_quantity"); //productQuan =
	 * productQuan.replaceAll("[^A-Za-z0-9 ]", ""); System.out.println(productQuan);
	 * Scanner s = new Scanner(productQuan); String quan; String unitCase = null;
	 * int units = 0; int cases = 0; while (s.hasNext()) { quan = unitCase; unitCase
	 * = s.next(); if (unitCase.toLowerCase().contains("unit")) {
	 * quan.replaceAll(",", ""); try { units += Integer.parseInt(quan); } catch
	 * (Exception e) { // System.out.println("QUAN Error:\t" + quan); }
	 * //System.out.println("QUAN: " + quan);
	 * 
	 * } if (unitCase.toLowerCase().contains("case")) { unitCase.replaceAll(",",
	 * ""); try { cases += Integer.parseInt(unitCase); } catch (Exception e) { //
	 * System.out.println("UNIT/CASE Error:\t" + unitCase); }
	 * //System.out.println("UNIT/CASE:\t" + unitCase); } } s.close();
	 * //System.out.println("Units:\t" + units); //System.out.println("Cases:\t" +
	 * cases);
	 * 
	 * jObj.put("units", Integer.toString(units)); jObj.put("cases",
	 * Integer.toString(cases)); }
	 * 
	 * @Override public void readFile() throws IOException, ParseException {
	 * 
	 * }
	 */

}
