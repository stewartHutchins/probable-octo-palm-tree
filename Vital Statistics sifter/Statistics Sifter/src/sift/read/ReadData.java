package sift.read;

public interface ReadData {

	/**
	 * Removes repeats in the data, if there are no repeats in the data source, then
	 * this method need not be implemented for new data sources
	 * 
	 * @throws Exception
	 *             throws exception for various reasons, e.g. file not found. The
	 *             message should be parsed up the stack so the user can read the
	 *             error
	 */
	public void removeRepeats() throws Exception;

	/**
	 * Reads a file with no repeats into memory, so key information can be returned
	 * 
	 * @throws Exception
	 *             throws exception for various reasons, e.g. file not found. The
	 *             message should be parsed up the stack so the user can read the
	 *             error
	 */
	public void readFile() throws Exception;

}
