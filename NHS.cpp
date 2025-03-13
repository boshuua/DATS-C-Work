#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

// Define structures
struct Treatment {
	string condition;
	string treatment;
	string frequency;
	double cost;
	string treatmentLength;
};

struct Patient {
	string username;
	string password;
	string name;
	int age;
	string currentCondition; // Cancer, Diabetes, Smoking, Multiple, None
	int cancerStage; // 1-4, 0 if no cancer
	int diabetesType; // 1-2, 0 if no diabetes
	string smokingFrequency; // "None", "Monthly", "Weekly", "Daily"
	bool previousCancer;
	bool previousSmoking;
	vector<Treatment> treatments;
};

struct User {
	string username;
	string password;
	string accessLevel; // "patient", "nurse", "doctor", "pharmacist"
	Patient* patientData; // NULL for non-patients
};

// Global variables
vector<User> users;
vector<Patient> patients;
User* currentUser = nullptr;
const double POUND_TO_DOLLAR = 1.26; // Conversion rate

// Function declarations
void saveData();
void loadData();
void initializeTreatmentCosts();
void displayLoginMenu();
void displayRegistrationMenu();
bool login(string username, string password);
void registerPatient();
void registerStaff(string accessLevel);
void displayPatientMenu();
void displayStaffMenu();
void viewPatientDetails(Patient* patient);
void addTreatment(Patient* patient);
void calculateStatistics();
void calculateTreatmentCost(Patient* patient);
double calculateDailyCost(Treatment t);
double calculateWeeklyCost(Treatment t);
double calculateMonthlyCost(Treatment t);
double calculateYearlyCost(Treatment t);
bool isValidCredentials(string username, string password);

// Main function
int main() {
	loadData();

	if (users.empty()) {
		// Create default admin accounts if none exist
		User admin;
		admin.username = "admin_doctor";
		admin.password = "password";
		admin.accessLevel = "doctor";
		admin.patientData = nullptr;
		users.push_back(admin);

		User adminPharmacist;
		adminPharmacist.username = "admin_pharmacist";
		adminPharmacist.password = "password";
		adminPharmacist.accessLevel = "pharmacist";
		adminPharmacist.patientData = nullptr;
		users.push_back(adminPharmacist);
	}

	while (true) {
		if (currentUser == nullptr) {
			displayLoginMenu();
		}
		else {
			if (currentUser->accessLevel == "patient") {
				displayPatientMenu();
			}
			else {
				displayStaffMenu();
			}
		}
	}

	return 0;
}

// Function to save data to file
void saveData() {
	ofstream userFile("users.csv");
	ofstream patientFile("patients.csv");
	ofstream treatmentFile("treatments.csv");

	// Save users
	for (auto user : users) {
		userFile << user.username << "," << user.password << "," << user.accessLevel;
		if (user.patientData != nullptr) {
			userFile << "," << user.patientData->name;
		}
		else {
			userFile << ",NULL";
		}
		userFile << endl;
	}

	// Save patients
	for (auto patient : patients) {
		patientFile << patient.username << "," << patient.password << "," << patient.name << ","
			<< patient.age << "," << patient.currentCondition << ","
			<< patient.cancerStage << "," << patient.diabetesType << ","
			<< patient.smokingFrequency << "," << patient.previousCancer << ","
			<< patient.previousSmoking << endl;

		// Save treatments
		for (auto treatment : patient.treatments) {
			treatmentFile << patient.username << "," << treatment.condition << ","
				<< treatment.treatment << "," << treatment.frequency << ","
				<< treatment.cost << "," << treatment.treatmentLength << endl;
		}
	}

	userFile.close();
	patientFile.close();
	treatmentFile.close();
}

// Function to load data from file
void loadData() {
	ifstream userFile("users.csv");
	ifstream patientFile("patients.csv");
	ifstream treatmentFile("treatments.csv");

	users.clear();
	patients.clear();

	// Load patients first
	string line;
	while (getline(patientFile, line)) {
		stringstream ss(line);
		string item;
		vector<string> items;

		while (getline(ss, item, ',')) {
			items.push_back(item);
		}

		if (items.size() >= 10) {
			Patient p;
			p.username = items[0];
			p.password = items[1];
			p.name = items[2];
			p.age = stoi(items[3]);
			p.currentCondition = items[4];
			p.cancerStage = stoi(items[5]);
			p.diabetesType = stoi(items[6]);
			p.smokingFrequency = items[7];
			p.previousCancer = (items[8] == "1");
			p.previousSmoking = (items[9] == "1");
			patients.push_back(p);
		}
	}

	// Load treatments
	while (getline(treatmentFile, line)) {
		stringstream ss(line);
		string item;
		vector<string> items;

		while (getline(ss, item, ',')) {
			items.push_back(item);
		}

		if (items.size() >= 6) {
			string patientUsername = items[0];
			for (auto& patient : patients) {
				if (patient.username == patientUsername) {
					Treatment t;
					t.condition = items[1];
					t.treatment = items[2];
					t.frequency = items[3];
					t.cost = stod(items[4]);
					t.treatmentLength = items[5];
					patient.treatments.push_back(t);
					break;
				}
			}
		}
	}

	// Load users last
	while (getline(userFile, line)) {
		stringstream ss(line);
		string item;
		vector<string> items;

		while (getline(ss, item, ',')) {
			items.push_back(item);
		}

		if (items.size() >= 4) {
			User u;
			u.username = items[0];
			u.password = items[1];
			u.accessLevel = items[2];

			if (items[3] != "NULL") {
				for (auto& patient : patients) {
					if (patient.name == items[3]) {
						u.patientData = &patient;
						break;
					}
				}
			}
			else {
				u.patientData = nullptr;
			}

			users.push_back(u);
		}
	}

	userFile.close();
	patientFile.close();
	treatmentFile.close();
}

// Initialize treatment costs based on the table provided
Treatment createTreatment(string condition, string treatment, string frequency, double cost, string treatmentLength) {
	Treatment t;
	t.condition = condition;
	t.treatment = treatment;
	t.frequency = frequency;
	t.cost = cost;
	t.treatmentLength = treatmentLength;
	return t;
}

// Function to display login menu
void displayLoginMenu() {
	int choice;
	string username, password;

	cout << "===== NHS North Somerset Patient Data System =====" << endl;
	cout << "1. Login" << endl;
	cout << "2. Register as new patient" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

	switch (choice) {
	case 1:
		cout << "Enter username: ";
		cin >> username;
		cout << "Enter password: ";
		cin >> password;
		if (login(username, password)) {
			cout << "Login successful!" << endl;
		}
		else {
			cout << "Invalid username or password!" << endl;
		}
		break;
	case 2:
		registerPatient();
		break;
	case 3:
		saveData();
		exit(0);
		break;
	default:
		cout << "Invalid choice!" << endl;
		break;
	}
}

// Function to handle login
bool login(string username, string password) {
	for (auto& user : users) {
		if (user.username == username && user.password == password) {
			currentUser = &user;
			return true;
		}
	}
	return false;
}

// Function to register a new patient
void registerPatient() {
	Patient newPatient;
	User newUser;

	cout << "===== Patient Registration =====" << endl;
	cout << "Enter username: ";
	cin >> newUser.username;

	// Check if username already exists
	for (auto user : users) {
		if (user.username == newUser.username) {
			cout << "Username already exists!" << endl;
			return;
		}
	}

	cout << "Enter password: ";
	cin >> newUser.password;

	if (!isValidCredentials(newUser.username, newUser.password)) {
		cout << "Invalid credentials! Username and password must be at least 4 characters." << endl;
		return;
	}

	newPatient.username = newUser.username;
	newPatient.password = newUser.password;

	cout << "Enter your full name: ";
	cin.ignore();
	getline(cin, newPatient.name);

	cout << "Enter your age: ";
	cin >> newPatient.age;

	cout << "Do you currently have cancer? (1 for Yes, 0 for No): ";
	int hasCancer;
	cin >> hasCancer;

	if (hasCancer) {
		newPatient.currentCondition = "Cancer";
		cout << "Enter cancer stage (1-4): ";
		cin >> newPatient.cancerStage;
	}
	else {
		newPatient.cancerStage = 0;
	}

	cout << "Do you currently have diabetes? (1 for Yes, 0 for No): ";
	int hasDiabetes;
	cin >> hasDiabetes;

	if (hasDiabetes) {
		if (hasCancer) {
			newPatient.currentCondition = "Multiple";
		}
		else {
			newPatient.currentCondition = "Diabetes";
		}
		cout << "Enter diabetes type (1 or 2): ";
		cin >> newPatient.diabetesType;
	}
	else {
		newPatient.diabetesType = 0;
	}

	cout << "Do you currently smoke? (1 for Yes, 0 for No): ";
	int smokes;
	cin >> smokes;

	if (smokes) {
		if (newPatient.currentCondition == "None") {
			newPatient.currentCondition = "Smoking";
		}
		else if (newPatient.currentCondition != "Multiple") {
			newPatient.currentCondition = "Multiple";
		}

		cout << "Enter smoking frequency (Monthly, Weekly, Daily): ";
		cin >> newPatient.smokingFrequency;
	}
	else {
		newPatient.smokingFrequency = "None";
	}

	if (!hasCancer && !hasDiabetes && !smokes) {
		newPatient.currentCondition = "None";
	}

	cout << "Have you previously had cancer? (1 for Yes, 0 for No): ";
	cin >> newPatient.previousCancer;

	cout << "Have you previously smoked? (1 for Yes, 0 for No): ";
	cin >> newPatient.previousSmoking;

	// Add treatments automatically based on conditions
	if (hasCancer && newPatient.cancerStage < 4) {
		Treatment t;
		t.condition = "Cancer";
		t.treatment = "Chemotherapy";

		if (newPatient.cancerStage == 1) {
			t.frequency = "Once every 4 weeks";
			t.treatmentLength = "For 6 months";
		}
		else if (newPatient.cancerStage == 2) {
			t.frequency = "Once every 2 weeks";
			t.treatmentLength = "For 6 months";
		}
		else if (newPatient.cancerStage == 3) {
			t.frequency = "Once every week";
			t.treatmentLength = "For 12 months";
		}

		t.cost = 10000.0;
		newPatient.treatments.push_back(t);
	}

	if (hasDiabetes) {
		Treatment t;
		t.condition = "Diabetes";
		t.treatment = "Insulin";

		if (newPatient.diabetesType == 1) {
			t.frequency = "2 shots per day";
		}
		else {
			t.frequency = "1 shot per day";
		}

		t.cost = 7.52;
		t.treatmentLength = "Forever";
		newPatient.treatments.push_back(t);
	}

	if (smokes) {
		Treatment t;
		t.condition = "Smoking";

		if (newPatient.smokingFrequency == "Monthly") {
			t.treatment = "Nicotine Tablets";
			t.frequency = "1 100mg pill a day";
			t.treatmentLength = "For 6 months";
			t.cost = 5.63 / 8.0; // Cost per tablet
		}
		else if (newPatient.smokingFrequency == "Weekly") {
			t.treatment = "Nicotine Tablets";
			t.frequency = "2 500mg pills a day";
			t.treatmentLength = "For 12 months";
			t.cost = 5.63 / 4.0; // Cost per tablet (2 pills per day)
		}
		else if (newPatient.smokingFrequency == "Daily") {
			t.treatment = "Nicotine Patch";
			t.frequency = "One every 24 hours";
			t.treatmentLength = "For 2 years";
			t.cost = 3.64;
		}

		newPatient.treatments.push_back(t);
	}

	// Add the new patient to the list
	patients.push_back(newPatient);

	// Create a user account for the patient
	newUser.accessLevel = "patient";
	newUser.patientData = &patients.back(); // Point to the newly added patient
	users.push_back(newUser);

	cout << "Registration successful!" << endl;
	saveData();
}

// Function to register staff (nurse, doctor, or pharmacist)
void registerStaff(string accessLevel) {
	User newUser;

	cout << "===== " << accessLevel << " Registration =====" << endl;
	cout << "Enter username: ";
	cin >> newUser.username;

	// Check if username already exists
	for (auto user : users) {
		if (user.username == newUser.username) {
			cout << "Username already exists!" << endl;
			return;
		}
	}

	cout << "Enter password: ";
	cin >> newUser.password;

	if (!isValidCredentials(newUser.username, newUser.password)) {
		cout << "Invalid credentials! Username and password must be at least 4 characters." << endl;
		return;
	}

	newUser.accessLevel = accessLevel;
	newUser.patientData = nullptr;

	users.push_back(newUser);
	cout << "Registration successful!" << endl;
	saveData();
}

// Function to display patient menu
void displayPatientMenu() {
	int choice;

	cout << "===== Patient Menu =====" << endl;
	cout << "Welcome, " << currentUser->patientData->name << endl;
	cout << "1. View my details" << endl;
	cout << "2. View my treatments and costs" << endl;
	cout << "3. Logout" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

	switch (choice) {
	case 1:
		viewPatientDetails(currentUser->patientData);
		break;
	case 2:
		calculateTreatmentCost(currentUser->patientData);
		break;
	case 3:
		currentUser = nullptr;
		cout << "Logged out successfully!" << endl;
		break;
	default:
		cout << "Invalid choice!" << endl;
		break;
	}
}

// Function to display staff menu
void displayStaffMenu() {
	int choice;
	string patientUsername;

	cout << "===== " << currentUser->accessLevel << " Menu =====" << endl;
	cout << "1. View patient list" << endl;
	cout << "2. View patient details" << endl;
	cout << "3. Add/modify patient treatment" << endl;
	cout << "4. View system statistics" << endl;

	if (currentUser->accessLevel == "doctor") {
		cout << "5. Register new nurse" << endl;
		cout << "6. Register new doctor" << endl;
	}
	else if (currentUser->accessLevel == "pharmacist") {
		cout << "5. Register new pharmacist" << endl;
	}

	cout << "7. Logout" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

	switch (choice) {
	case 1: {
		cout << "===== Patient List =====" << endl;
		for (int i = 0; i < patients.size(); i++) {
			cout << (i + 1) << ". " << patients[i].name << " (" << patients[i].username << ")" << endl;
		}
		break;
	}
	case 2: {
		cout << "Enter patient username: ";
		cin >> patientUsername;

		bool found = false;
		for (auto& patient : patients) {
			if (patient.username == patientUsername) {
				viewPatientDetails(&patient);
				found = true;
				break;
			}
		}

		if (!found) {
			cout << "Patient not found!" << endl;
		}
		break;
	}
	case 3: {
		cout << "Enter patient username: ";
		cin >> patientUsername;

		bool found = false;
		for (auto& patient : patients) {
			if (patient.username == patientUsername) {
				addTreatment(&patient);
				found = true;
				break;
			}
		}

		if (!found) {
			cout << "Patient not found!" << endl;
		}
		break;
	}
	case 4:
		calculateStatistics();
		break;
	case 5:
		if (currentUser->accessLevel == "doctor") {
			registerStaff("nurse");
		}
		else if (currentUser->accessLevel == "pharmacist") {
			registerStaff("pharmacist");
		}
		else {
			cout << "Invalid choice!" << endl;
		}
		break;
	case 6:
		if (currentUser->accessLevel == "doctor") {
			registerStaff("doctor");
		}
		else {
			cout << "Invalid choice!" << endl;
		}
		break;
	case 7:
		currentUser = nullptr;
		cout << "Logged out successfully!" << endl;
		break;
	default:
		cout << "Invalid choice!" << endl;
		break;
	}
}

// Function to view patient details
void viewPatientDetails(Patient* patient) {
	cout << "===== Patient Details =====" << endl;
	cout << "Name: " << patient->name << endl;
	cout << "Age: " << patient->age << endl;
	cout << "Current Condition: " << patient->currentCondition << endl;

	if (patient->cancerStage > 0) {
		cout << "Cancer Stage: " << patient->cancerStage << endl;
	}

	if (patient->diabetesType > 0) {
		cout << "Diabetes Type: " << patient->diabetesType << endl;
	}

	if (patient->smokingFrequency != "None") {
		cout << "Smoking Frequency: " << patient->smokingFrequency << endl;
	}

	cout << "Medical History:" << endl;
	cout << "- Previous Cancer: " << (patient->previousCancer ? "Yes" : "No") << endl;
	cout << "- Previous Smoking: " << (patient->previousSmoking ? "Yes" : "No") << endl;

	cout << "Current Treatments:" << endl;
	if (patient->treatments.empty()) {
		cout << "No active treatments." << endl;
	}
	else {
		for (auto& treatment : patient->treatments) {
			cout << "- Condition: " << treatment.condition << endl;
			cout << "  Treatment: " << treatment.treatment << endl;
			cout << "  Frequency: " << treatment.frequency << endl;
			cout << "  Cost: £" << treatment.cost << endl;
			cout << "  Length: " << treatment.treatmentLength << endl;
			cout << endl;
		}
	}
}

// Function to add or modify treatment
void addTreatment(Patient* patient) {
	int choice;
	bool hasCancer = false;

	// Check if patient already has cancer treatment
	for (auto& treatment : patient->treatments) {
		if (treatment.condition == "Cancer") {
			hasCancer = true;
			break;
		}
	}

	cout << "===== Add/Modify Treatment =====" << endl;
	cout << "1. Add cancer treatment" << endl;
	cout << "2. Add diabetes treatment" << endl;
	cout << "3. Add smoking cessation treatment" << endl;
	cout << "4. Remove treatment" << endl;
	cout << "5. Back" << endl;
	cout << "Enter your choice: ";
	cin >> choice;

	switch (choice) {
	case 1: {
		if (patient->cancerStage == 0) {
			cout << "Patient does not have cancer!" << endl;
			break;
		}

		if (patient->cancerStage == 4) {
			cout << "Stage 4 cancer is terminal and has no treatment." << endl;
			break;
		}

		// Cannot add cancer treatment if patient already has other treatments
		if (!patient->treatments.empty() && !hasCancer) {
			cout << "Cannot add chemotherapy with other treatments!" << endl;
			break;
		}

		// Remove existing cancer treatment if any
		for (int i = 0; i < patient->treatments.size(); i++) {
			if (patient->treatments[i].condition == "Cancer") {
				patient->treatments.erase(patient->treatments.begin() + i);
				break;
			}
		}

		Treatment t;
		t.condition = "Cancer";
		t.treatment = "Chemotherapy";

		if (patient->cancerStage == 1) {
			t.frequency = "Once every 4 weeks";
			t.treatmentLength = "For 6 months";
		}
		else if (patient->cancerStage == 2) {
			t.frequency = "Once every 2 weeks";
			t.treatmentLength = "For 6 months";
		}
		else if (patient->cancerStage == 3) {
			t.frequency = "Once every week";
			t.treatmentLength = "For 12 months";
		}

		t.cost = 10000.0;
		patient->treatments.push_back(t);

		cout << "Cancer treatment added/modified successfully!" << endl;
		break;
	}
	case 2: {
		if (patient->diabetesType == 0) {
			cout << "Patient does not have diabetes!" << endl;
			break;
		}

		// Cannot add diabetes treatment if patient has cancer treatment
		if (hasCancer) {
			cout << "Cannot add insulin with chemotherapy!" << endl;
			break;
		}

		// Remove existing diabetes treatment if any
		for (int i = 0; i < patient->treatments.size(); i++) {
			if (patient->treatments[i].condition == "Diabetes") {
				patient->treatments.erase(patient->treatments.begin() + i);
				break;
			}
		}

		Treatment t;
		t.condition = "Diabetes";
		t.treatment = "Insulin";

		if (patient->diabetesType == 1) {
			t.frequency = "2 shots per day";
		}
		else {
			t.frequency = "1 shot per day";
		}

		t.cost = 7.52;
		t.treatmentLength = "Forever";
		patient->treatments.push_back(t);

		cout << "Diabetes treatment added/modified successfully!" << endl;
		break;
	}
	case 3: {
		if (patient->smokingFrequency == "None") {
			cout << "Patient does not smoke!" << endl;
			break;
		}

		// Cannot add smoking treatment if patient has cancer treatment
		if (hasCancer) {
			cout << "Cannot add smoking treatment with chemotherapy!" << endl;
			break;
		}

		// Remove existing smoking treatment if any
		for (int i = 0; i < patient->treatments.size(); i++) {
			if (patient->treatments[i].condition == "Smoking") {
				patient->treatments.erase(patient->treatments.begin() + i);
				break;
			}
		}

		Treatment t;
		t.condition = "Smoking";

		if (patient->smokingFrequency == "Monthly") {
			t.treatment = "Nicotine Tablets";
			t.frequency = "1 100mg pill a day";
			t.treatmentLength = "For 6 months";
			t.cost = 5.63 / 8.0; // Cost per tablet
		}
		else if (patient->smokingFrequency == "Weekly") {
			t.treatment = "Nicotine Tablets";
			t.frequency = "2 500mg pills a day";
			t.treatmentLength = "For 12 months";
			t.cost = 5.63 / 4.0; // Cost per tablet (2 pills per day)
		}
		else if (patient->smokingFrequency == "Daily") {
			t.treatment = "Nicotine Patch";
			t.frequency = "One every 24 hours";
			t.treatmentLength = "For 2 years";
			t.cost = 3.64;
		}

		patient->treatments.push_back(t);

		cout << "Smoking treatment added/modified successfully!" << endl;
		break;
	}
	case 4: {
		if (patient->treatments.empty()) {
			cout << "Patient has no treatments to remove!" << endl;
			break;
		}

		cout << "Select treatment to remove:" << endl;
		for (int i = 0; i < patient->treatments.size(); i++) {
			cout << (i + 1) << ". " << patient->treatments[i].condition << " - "
				<< patient->treatments[i].treatment << endl;
		}

		int treatmentIndex;
		cout << "Enter treatment number: ";
		cin >> treatmentIndex;

		if (treatmentIndex >= 1 && treatmentIndex <= patient->treatments.size()) {
			patient->treatments.erase(patient->treatments.begin() + treatmentIndex - 1);
			cout << "Treatment removed successfully!" << endl;
		}
		else {
			cout << "Invalid treatment number!" << endl;
		}
		break;
	}
	case 5:
		break;
	default:
		cout << "Invalid choice!" << endl;
		break;
	}

	saveData();
}

// Function to calculate statistics
void calculateStatistics() {
	int cancerCount = 0;
	int diabetesType1Count = 0;
	int diabetesType2Count = 0;
	int cancerRecoveredCount = 0;
	int smokersWithCancerCount = 0;
	int smokersCount = 0;

	int cancerAgeSum = 0;
	int diabetesType1AgeSum = 0;
	int diabetesType2AgeSum = 0;
	int cancerRecoveredAgeSum = 0;

	for (auto& patient : patients) {
		if (patient.cancerStage > 0) {
			cancerCount++;
			cancerAgeSum += patient.age;

			if (patient.smokingFrequency != "None") {
				smokersWithCancerCount++;
			}
		}

		if (patient.previousCancer) {
			cancerRecoveredCount++;
			cancerRecoveredAgeSum += patient.age;
		}

		if (patient.diabetesType == 1) {
			diabetesType1Count++;
			diabetesType1AgeSum += patient.age;
		}
		else if (patient.diabetesType == 2) {
			diabetesType2Count++;
			diabetesType2AgeSum += patient.age;
		}

		if (patient.smokingFrequency != "None") {
			smokersCount++;
		}
	}

	cout << "===== System Statistics =====" << endl;

	if (cancerCount > 0) {
		cout << "Average age of patients with cancer: " << (double)cancerAgeSum / cancerCount << endl;
	}
	else {
		cout << "No patients with cancer." << endl;
	}

	if (cancerRecoveredCount > 0) {
		cout << "Average age of patients who recovered from cancer: " << (double)cancerRecoveredAgeSum / cancerRecoveredCount << endl;
	}
	else {
		cout << "No patients who recovered from cancer." << endl;
	}

	if (diabetesType1Count > 0) {
		cout << "Average age of patients with Type 1 diabetes: " << (double)diabetesType1AgeSum / diabetesType1Count << endl;
	}
	else {
		cout << "No patients with Type 1 diabetes." << endl;
	}

	if (diabetesType2Count > 0) {
		cout << "Average age of patients with Type 2 diabetes: " << (double)diabetesType2AgeSum / diabetesType2Count << endl;
	}
	else {
		cout << "No patients with Type 2 diabetes." << endl;
	}

	if (smokersCount > 0) {
		cout << "Percentage of smokers with cancer: " << (double)smokersWithCancerCount / smokersCount * 100 << "%" << endl;
	}
	else {
		cout << "No smoking patients." << endl;
	}
}
// Function to calculate treatment cost
void calculateTreatmentCost(Patient* patient) {
	double dailyCost = 0.0;
	double weeklyCost = 0.0;
	double monthlyCost = 0.0;
	double yearlyCost = 0.0;

	cout << "===== Treatment Costs for " << patient->name << " =====" << endl;

	if (patient->treatments.empty()) {
		cout << "No active treatments." << endl;
		return;
	}

	for (auto& treatment : patient->treatments) {
		cout << "Treatment: " << treatment.condition << " - " << treatment.treatment << endl;
		cout << "Frequency: " << treatment.frequency << endl;

		double treatmentDailyCost = calculateDailyCost(treatment);
		double treatmentWeeklyCost = calculateWeeklyCost(treatment);
		double treatmentMonthlyCost = calculateMonthlyCost(treatment);
		double treatmentYearlyCost = calculateYearlyCost(treatment);

		dailyCost += treatmentDailyCost;
		weeklyCost += treatmentWeeklyCost;
		monthlyCost += treatmentMonthlyCost;
		yearlyCost += treatmentYearlyCost;

		cout << "Daily cost: £" << fixed << setprecision(2) << treatmentDailyCost << " ($" << treatmentDailyCost * POUND_TO_DOLLAR << ")" << endl;
		cout << "Weekly cost: £" << fixed << setprecision(2) << treatmentWeeklyCost << " ($" << treatmentWeeklyCost * POUND_TO_DOLLAR << ")" << endl;
		cout << "Monthly cost: £" << fixed << setprecision(2) << treatmentMonthlyCost << " ($" << treatmentMonthlyCost * POUND_TO_DOLLAR << ")" << endl;
		cout << "Yearly cost: £" << fixed << setprecision(2) << treatmentYearlyCost << " ($" << treatmentYearlyCost * POUND_TO_DOLLAR << ")" << endl;
		cout << endl;
	}

	cout << "===== Total Treatment Costs =====" << endl;
	cout << "Daily cost: £" << fixed << setprecision(2) << dailyCost << " ($" << dailyCost * POUND_TO_DOLLAR << ")" << endl;
	cout << "Weekly cost: £" << fixed << setprecision(2) << weeklyCost << " ($" << weeklyCost * POUND_TO_DOLLAR << ")" << endl;
	cout << "Monthly cost: £" << fixed << setprecision(2) << monthlyCost << " ($" << monthlyCost * POUND_TO_DOLLAR << ")" << endl;
	cout << "Yearly cost: £" << fixed << setprecision(2) << yearlyCost << " ($" << yearlyCost * POUND_TO_DOLLAR << ")" << endl;
}

// Function to calculate daily cost for a treatment
double calculateDailyCost(Treatment t) {
	if (t.condition == "Cancer") {
		if (t.frequency == "Once every 4 weeks") {
			return t.cost / 28.0;
		}
		else if (t.frequency == "Once every 2 weeks") {
			return t.cost / 14.0;
		}
		else if (t.frequency == "Once every week") {
			return t.cost / 7.0;
		}
	}
	else if (t.condition == "Diabetes") {
		if (t.frequency == "2 shots per day") {
			return t.cost * 2.0;
		}
		else if (t.frequency == "1 shot per day") {
			return t.cost;
		}
	}
	else if (t.condition == "Smoking") {
		if (t.frequency == "1 100mg pill a day") {
			return t.cost;
		}
		else if (t.frequency == "2 500mg pills a day") {
			return t.cost * 2.0;
		}
		else if (t.frequency == "One every 24 hours") {
			return t.cost;
		}
	}

	return 0.0;
}

// Function to calculate weekly cost for a treatment
double calculateWeeklyCost(Treatment t) {
	return calculateDailyCost(t) * 7.0;
}

// Function to calculate monthly cost for a treatment
double calculateMonthlyCost(Treatment t) {
	return calculateDailyCost(t) * 30.0;
}

// Function to calculate yearly cost for a treatment
double calculateYearlyCost(Treatment t) {
	return calculateDailyCost(t) * 365.0;
}

// Function to validate credentials
bool isValidCredentials(string username, string password) {
	// Simple validation: username and password must be at least 4 characters
	return username.length() >= 4 && password.length() >= 4;
}