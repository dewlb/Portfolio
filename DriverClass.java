// Ethan Jewell

import java.util.ArrayList;
import java.util.*;

public class DriverClass {
	public static void main(String[] args) {
		
		String fullName = "Erika T. Jones";
		String employeeNumber = "ej789";
		double payRate = 100.0, hoursWorked = 1.0;

		Employee e;
		e = new Employee(fullName, employeeNumber, payRate, hoursWorked);
		
		System.out.println(e); // To Test your toString method
		
		e.printCheck(); // This prints the check of Erika T. Jones

		Company company = new Company();
		
		company.hire ( new Employee ("Saeed Happy", "sh895" , 2 , 200) );
		company.hire (e);
		company.printCompanyInfo();
		company.hire( new Employee("Enrico Torres" , "et897" , 3 , 150) );

		company.printCheck("ab784");
		company.deleteEmployeesBySalary(256.36);
		company.reverseEmployees();
		System.out.println( company.SearchByName("WaLiD WiLLiAms") );
		company.printEmployees();
		System.out.println("Bye!");
	}
}

// This is our employee class to store the information we will need
class Employee {
	
	private String fullName;
	private String employeeNumber;
	private double payRate;
	private double hoursWorked;
	
	// This is our constructor for the employee
	public Employee(String fullName, String employeeNumber, double payRate, double hoursWorked) {
		this.fullName = fullName;
		this.employeeNumber = employeeNumber;
		this.payRate = payRate;
		this.hoursWorked = hoursWorked;
	}
	
	// Gets fullName
	public String getFullName() {
		return fullName;
	}

	// Sets fullName
	public void setFullName(String fullName) {
		this.fullName = fullName;
	}

	// Gets employeeNumber
	public String getEmployeeNumber() {
		return employeeNumber;
	}

	// Sets employeeNumber
	public void setEmployeeNumber(String employeeNumber) {
		this.employeeNumber = employeeNumber;
	}

	// Gets payRate
	public double getPayRate() {
		return payRate;
	}

	// Sets payRate
	public void setPayRate(double payRate) {
		this.payRate = payRate;
	}

	// Gets hoursWorked
	public double getHoursWorked() {
		return hoursWorked;
	}

	// Sets hoursWorked
	public void setHoursWorked(double hoursWorked) {
		this.hoursWorked = hoursWorked;
	}
	
	// The default print formatting for an employee
	public String toString() {
		return "[" + employeeNumber + "/" + fullName + ", " + hoursWorked + " Hours @ " + payRate + " per hour]";
	}
	
	// Calculates the net pay of employee with included tax
	private double netPay() {
		return (payRate * hoursWorked) * 0.94;
	}
	
	// Prints out a check based on current employee
	public void printCheck() {
		System.out.println("---------------------------------------------------------------------------");
		System.out.println("	Employee's name:       " + fullName);
		System.out.println("	Employee's number:     " + employeeNumber);
		System.out.println("	Hourly rate of pay:    " + payRate);
		System.out.println("	Hours worked:          " + hoursWorked);
		System.out.println("");
		System.out.printf("	Total Gross Pay:       $%.2f\n", (payRate * hoursWorked));
		System.out.println("");
		System.out.println("	Deductions:");
		System.out.printf("	Tax (6%%):              $%.2f\n", ((payRate * hoursWorked) * 0.06));
		System.out.println("                           ");
		System.out.printf("	Net Pay:               %.2f Dollars\n", netPay());
		System.out.println("");
		System.out.println("---------------------------------------------------------------------------");
	}
	
}

class Company {
	
	private ArrayList<Employee> employeeList;
	private String companyName;
	private static String companyTaxId;
	
	// Get companyName
	public String getCompanyName() {
		return companyName;
	}

	// Set companyName
	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}

	// Get companyTaxId
	public static String getCompanyTaxId() {
		return companyTaxId;
	}

	// Set companyTexId
	public static void setCompanyTaxId(String companyTaxId) {
		Company.companyTaxId = companyTaxId;
	}
	
	// Default constructor for company
	public Company() {
		employeeList = new ArrayList<>();
		companyName = "People's Place";
		companyTaxId = "v1rtua7C0mpan1";
	}
	
	// Loop through current list to make sure there are no dupes then add
	public boolean hire ( Employee employee ) {
		for (Employee e : employeeList) {
			if (employee.getEmployeeNumber() == e.getEmployeeNumber()) {
				return false;
			}
		}
		
		employeeList.add(employee);
		return true;
	}
	
	// Prints out a formatted list of company information
	public void printCompanyInfo() {
		System.out.println("Company Name: " + companyName + " || Tax ID: " + companyTaxId + " || Current Employee Count: " + employeeList.size());
	}
	
	// Prints the information of each employee in the company
	public void printEmployees() {
		for (Employee e : employeeList) {
			System.out.println(e.toString());
		}
	}
	
	// Keep track of the count of employees who are paid less than the max salary and return it
	public int countEmployees( double maxSalary ) {
		int count = 0;
		
		for (Employee e : employeeList) {
			if (e.getPayRate() < maxSalary) {
				count++;
			}
		}
		
		return count;
	}
	
	// Search to see if a specific employee name exists in the list
	public boolean SearchByName (String fullName ) {
		for (Employee e : employeeList) {
			if (fullName.toLowerCase().equals(e.getFullName().toLowerCase())) {
				return true;
			}
		}
		
		return false;
	}
	
	// Reverse the order of the employee list
	public void reverseEmployees () {
		Collections.reverse(employeeList);
	}

	// Delete employees with the same salary as the target
	public void deleteEmployeesBySalary (double targetSalary ) {
		for (int i = 0; i < employeeList.size(); i++) {
			if (employeeList.get(i).getPayRate() == targetSalary) {
				employeeList.remove(i);
				i--;
			}
		}
	}
	
	// Prints the check of the employee who matches the employee number given, if any
	public void printCheck ( String employeeNumber) {
		for (Employee e : employeeList) {
			if (e.getEmployeeNumber() == employeeNumber) {
				e.printCheck();
				return;
			}
		}
		
		System.out.println("NO SUCH EMPLOYEE EXISTS");
	}
}
