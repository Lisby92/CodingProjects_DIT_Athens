-- MySQL Script generated by MySQL Workbench
-- Sat Mar 31 16:26:01 2018
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `mydb`.`Police_Department`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Police_Department` (
  `Police_Department_code` INT NOT NULL,
  `Police_Department_name` VARCHAR(45) NOT NULL,
  `Police_Department_tel` VARCHAR(45) NOT NULL,
  `Police_Department_address` VARCHAR(45) NOT NULL,
  `Police_Department_postcode` INT NOT NULL,
  `Police_Department_town` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Police_Department_code`),
  UNIQUE INDEX `Police_Department_code_UNIQUE` (`Police_Department_code` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Police_Officers`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Police_Officers` (
  `Police_Officers_badge_num` INT NOT NULL,
  `Police_Officers_name` VARCHAR(45) NOT NULL,
  `Police_Officers_rank` VARCHAR(45) NOT NULL,
  `Police_Department_code` INT NOT NULL,
  `Police_Officers_badge_num_1` INT NOT NULL,
  PRIMARY KEY (`Police_Officers_badge_num`, `Police_Officers_badge_num_1`),
  UNIQUE INDEX `Police_Officers_badge_num_UNIQUE` (`Police_Officers_badge_num` ASC),
  INDEX `fk_Police_Officers_Police_Department_idx` (`Police_Department_code` ASC),
  INDEX `fk_Police_Officers_Police_Officers1_idx` (`Police_Officers_badge_num_1` ASC),
  CONSTRAINT `fk_Police_Officers_Police_Department`
    FOREIGN KEY (`Police_Department_code`)
    REFERENCES `mydb`.`Police_Department` (`Police_Department_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Police_Officers_Police_Officers1`
    FOREIGN KEY (`Police_Officers_badge_num_1`)
    REFERENCES `mydb`.`Police_Officers` (`Police_Officers_badge_num`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Road_Details`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Road_Details` (
  `Road_Details_code` INT NOT NULL,
  `Road_Details_type` VARCHAR(45) NOT NULL,
  `Road_Details_condition` VARCHAR(45) NOT NULL,
  `Road_Details_speed_limit` INT NOT NULL,
  PRIMARY KEY (`Road_Details_code`),
  UNIQUE INDEX `Road_Details_code_UNIQUE` (`Road_Details_code` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Report`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Report` (
  `Report_prot_num` INT NOT NULL,
  `Report_register_date` DATE NOT NULL,
  `Report_revision_date` DATE NOT NULL,
  `Report_description` LONGTEXT NOT NULL,
  `Report_coordinates` VARCHAR(45) NOT NULL,
  `Police_Department_code` INT NOT NULL,
  `Road_Details_code` INT NOT NULL,
  `Police_Officers_badge_num` INT NOT NULL,
  `Police_Officers_badge_num_1` INT NOT NULL,
  PRIMARY KEY (`Report_prot_num`),
  UNIQUE INDEX `Report_prot_num_UNIQUE` (`Report_prot_num` ASC),
  INDEX `fk_Report_Police_Department1_idx` (`Police_Department_code` ASC),
  INDEX `fk_Report_Road_Details1_idx` (`Road_Details_code` ASC),
  INDEX `fk_Report_Police_Officers1_idx` (`Police_Officers_badge_num` ASC, `Police_Officers_badge_num_1` ASC),
  CONSTRAINT `fk_Report_Police_Department1`
    FOREIGN KEY (`Police_Department_code`)
    REFERENCES `mydb`.`Police_Department` (`Police_Department_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Report_Road_Details1`
    FOREIGN KEY (`Road_Details_code`)
    REFERENCES `mydb`.`Road_Details` (`Road_Details_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Report_Police_Officers1`
    FOREIGN KEY (`Police_Officers_badge_num` , `Police_Officers_badge_num_1`)
    REFERENCES `mydb`.`Police_Officers` (`Police_Officers_badge_num` , `Police_Officers_badge_num_1`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Vehicle_Registration`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Vehicle_Registration` (
  `Vehicle_Registration_code` INT NOT NULL,
  `Vehicle_Registration_model` VARCHAR(45) NOT NULL,
  `Vehicle_Registration_colour` VARCHAR(45) NOT NULL,
  `Vehicle_Registration_frame_number` VARCHAR(45) NOT NULL,
  `Vehicle_Registration_release_date` DATE NOT NULL,
  `Vehicle_Registration_manufacturer` VARCHAR(45) NOT NULL,
  `Vehicle_Registration_cylinder_capacity` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Vehicle_Registration_code`),
  UNIQUE INDEX `Vehicle_Registration_code_UNIQUE` (`Vehicle_Registration_code` ASC),
  UNIQUE INDEX `Vehicle_Registration_frame_number_UNIQUE` (`Vehicle_Registration_frame_number` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Insurance_company`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Insurance_company` (
  `Insurance_company_code` INT NOT NULL,
  `Insurance_company_name` VARCHAR(45) NOT NULL,
  `Insurance_cor_tel` VARCHAR(45) NOT NULL,
  `Insurance_cor_address` VARCHAR(45) NOT NULL,
  `Insurance_cor_email` VARCHAR(45) NOT NULL,
  `Insurance_cor_parentcode` INT NULL,
  PRIMARY KEY (`Insurance_company_code`),
  UNIQUE INDEX `Insurance_cor_code_UNIQUE` (`Insurance_company_code` ASC),
  UNIQUE INDEX `Insurance_cor_parentcode_UNIQUE` (`Insurance_cor_parentcode` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Trading_company`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Trading_company` (
  `Trading_company_code` INT NOT NULL,
  `Trading_company_name` VARCHAR(45) NOT NULL,
  `Trading_company_address` VARCHAR(45) NOT NULL,
  `Trading_company_tel` VARCHAR(45) NOT NULL,
  `Trading_company_email` VARCHAR(45) NOT NULL,
  `Trading_company_parentcode` INT NULL,
  PRIMARY KEY (`Trading_company_code`),
  UNIQUE INDEX `Trading_company_code_UNIQUE` (`Trading_company_code` ASC),
  UNIQUE INDEX `Trading_company_parentcode_UNIQUE` (`Trading_company_parentcode` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Transportation`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Transportation` (
  `Transportation_file_code` INT NOT NULL,
  `Transportation_type` VARCHAR(45) NOT NULL,
  `Transportation_weight` VARCHAR(45) NOT NULL,
  `Transportation_code` VARCHAR(45) NOT NULL,
  `Transportation_danger_rate` INT NULL,
  `Transportation_max_danger` INT NOT NULL,
  `Trading_company_code` INT NOT NULL,
  PRIMARY KEY (`Transportation_file_code`, `Trading_company_code`),
  UNIQUE INDEX `Transportation_file_code_UNIQUE` (`Transportation_file_code` ASC),
  UNIQUE INDEX `Transportation_code_UNIQUE` (`Transportation_code` ASC),
  INDEX `fk_Transportation_Trading_company1_idx` (`Trading_company_code` ASC),
  CONSTRAINT `fk_Transportation_Trading_company1`
    FOREIGN KEY (`Trading_company_code`)
    REFERENCES `mydb`.`Trading_company` (`Trading_company_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Vehicles`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Vehicles` (
  `Vehicles_code` INT NOT NULL,
  `Vehicles_bodywork` VARCHAR(45) NOT NULL,
  `Vehicles_estimatedspeed` FLOAT NULL,
  `Vehicles_passengers` INT NULL,
  `Vehicle_Registration_code` INT NOT NULL,
  `Insurance_company_code` INT NULL,
  `Transportation_file_code` INT NULL,
  `Transportation_Trading_company_code` INT NULL,
  `Report_prot_num` INT NOT NULL,
  PRIMARY KEY (`Vehicles_code`),
  UNIQUE INDEX `Vehicles_code_UNIQUE` (`Vehicles_code` ASC),
  INDEX `fk_Vehicles_Vehicle_Registration1_idx` (`Vehicle_Registration_code` ASC),
  INDEX `fk_Vehicles_Insurance_company1_idx` (`Insurance_company_code` ASC),
  INDEX `fk_Vehicles_Transportation1_idx` (`Transportation_file_code` ASC, `Transportation_Trading_company_code` ASC),
  INDEX `fk_Vehicles_Report1_idx` (`Report_prot_num` ASC),
  CONSTRAINT `fk_Vehicles_Vehicle_Registration1`
    FOREIGN KEY (`Vehicle_Registration_code`)
    REFERENCES `mydb`.`Vehicle_Registration` (`Vehicle_Registration_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Vehicles_Insurance_company1`
    FOREIGN KEY (`Insurance_company_code`)
    REFERENCES `mydb`.`Insurance_company` (`Insurance_company_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Vehicles_Transportation1`
    FOREIGN KEY (`Transportation_file_code` , `Transportation_Trading_company_code`)
    REFERENCES `mydb`.`Transportation` (`Transportation_file_code` , `Trading_company_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Vehicles_Report1`
    FOREIGN KEY (`Report_prot_num`)
    REFERENCES `mydb`.`Report` (`Report_prot_num`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Identity_card`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Identity_card` (
  `Identity_card_code` INT NOT NULL,
  `Identity_card_firstname` VARCHAR(45) NOT NULL,
  `Identity_cards_lastname` VARCHAR(45) NOT NULL,
  `Identity_card_address` VARCHAR(45) NOT NULL,
  `Identity_card_tel` VARCHAR(45) NOT NULL,
  `Identity_cards_dateofbirth` DATE NOT NULL,
  `Identity_card_AFM` INT NOT NULL,
  `Identity_cards_AMKA` INT NOT NULL,
  PRIMARY KEY (`Identity_card_code`),
  UNIQUE INDEX `Identity_cards_code_UNIQUE` (`Identity_card_code` ASC),
  UNIQUE INDEX `Identity_cards_AFM_UNIQUE` (`Identity_card_AFM` ASC),
  UNIQUE INDEX `Identity_cards_AMKA_UNIQUE` (`Identity_cards_AMKA` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Hospital`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Hospital` (
  `Hospital_code` INT NOT NULL,
  `Hospital_name` VARCHAR(45) NOT NULL,
  `Hospital_city` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Hospital_code`),
  UNIQUE INDEX `Hospital_code_UNIQUE` (`Hospital_code` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Role`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Role` (
  `Role_code` INT NOT NULL,
  `Role_description` TEXT(60) NOT NULL,
  PRIMARY KEY (`Role_code`),
  UNIQUE INDEX `Role_code_UNIQUE` (`Role_code` ASC),
  UNIQUE INDEX `Role_description_UNIQUE` (`Role_description` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Person`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Person` (
  `Person_code` INT NOT NULL,
  `Person_age` INT NOT NULL,
  `Person_position` VARCHAR(45) NULL,
  `Person_safety_measures` VARCHAR(45) NULL,
  `Person_violation` VARCHAR(45) NULL,
  `Identity_card_code` INT NOT NULL,
  `Hospital_code` INT NOT NULL,
  `Vehicles_code` INT NULL,
  `Role_code` INT NOT NULL,
  `Report_prot_num` INT NOT NULL,
  PRIMARY KEY (`Person_code`, `Identity_card_code`),
  UNIQUE INDEX `Person_code_UNIQUE` (`Person_code` ASC),
  INDEX `fk_Person_Identity_card1_idx` (`Identity_card_code` ASC),
  INDEX `fk_Person_Hospital1_idx` (`Hospital_code` ASC),
  INDEX `fk_Person_Vehicles1_idx` (`Vehicles_code` ASC),
  INDEX `fk_Person_Role1_idx` (`Role_code` ASC),
  INDEX `fk_Person_Report1_idx` (`Report_prot_num` ASC),
  CONSTRAINT `fk_Person_Identity_card1`
    FOREIGN KEY (`Identity_card_code`)
    REFERENCES `mydb`.`Identity_card` (`Identity_card_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Person_Hospital1`
    FOREIGN KEY (`Hospital_code`)
    REFERENCES `mydb`.`Hospital` (`Hospital_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Person_Vehicles1`
    FOREIGN KEY (`Vehicles_code`)
    REFERENCES `mydb`.`Vehicles` (`Vehicles_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Person_Role1`
    FOREIGN KEY (`Role_code`)
    REFERENCES `mydb`.`Role` (`Role_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Person_Report1`
    FOREIGN KEY (`Report_prot_num`)
    REFERENCES `mydb`.`Report` (`Report_prot_num`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Driving_Licence`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Driving_Licence` (
  `Driving_Licence_code` INT NOT NULL,
  `Driving_Licence_ethnicity` VARCHAR(45) NOT NULL,
  `Driving_Licence_category` VARCHAR(45) NOT NULL,
  `Driving_Licence_release_date` DATE NOT NULL,
  `Person_code` INT NOT NULL,
  `Person_Identity_card_code` INT NOT NULL,
  PRIMARY KEY (`Driving_Licence_code`, `Person_code`, `Person_Identity_card_code`),
  UNIQUE INDEX `Driving_Licence_code_UNIQUE` (`Driving_Licence_code` ASC),
  INDEX `fk_Driving_Licence_Person1_idx` (`Person_code` ASC, `Person_Identity_card_code` ASC),
  CONSTRAINT `fk_Driving_Licence_Person1`
    FOREIGN KEY (`Person_code` , `Person_Identity_card_code`)
    REFERENCES `mydb`.`Person` (`Person_code` , `Identity_card_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Corporation`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Corporation` (
  `Corporation_code` INT NOT NULL,
  `Corporation_name` VARCHAR(45) NOT NULL,
  `Corporation_address` TEXT(60) NOT NULL,
  `Corporation_tel` VARCHAR(45) NOT NULL,
  `Corporation_email` VARCHAR(45) NOT NULL,
  `Corporation_parent_code` INT NULL,
  PRIMARY KEY (`Corporation_code`),
  UNIQUE INDEX `Corporation_code_UNIQUE` (`Corporation_code` ASC),
  UNIQUE INDEX `Corporation_parent_code_UNIQUE` (`Corporation_parent_code` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Accident_report`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Accident_report` (
  `Accident_report_code` INT NOT NULL,
  `Accident_report_date` DATE NOT NULL,
  `Accident_report_estimated_damage` VARCHAR(45) NOT NULL,
  `Accident_report_estimated_compensation` INT NOT NULL,
  `Insurance_company_code` INT NOT NULL,
  `Report_prot_num` INT NOT NULL,
  `Vehicles_code` INT NOT NULL,
  PRIMARY KEY (`Accident_report_code`, `Insurance_company_code`),
  UNIQUE INDEX `Accident_report_code_UNIQUE` (`Accident_report_code` ASC),
  INDEX `fk_Accident_report_Insurance_company1_idx` (`Insurance_company_code` ASC),
  INDEX `fk_Accident_report_Report1_idx` (`Report_prot_num` ASC),
  INDEX `fk_Accident_report_Vehicles1_idx` (`Vehicles_code` ASC),
  CONSTRAINT `fk_Accident_report_Insurance_company1`
    FOREIGN KEY (`Insurance_company_code`)
    REFERENCES `mydb`.`Insurance_company` (`Insurance_company_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Accident_report_Report1`
    FOREIGN KEY (`Report_prot_num`)
    REFERENCES `mydb`.`Report` (`Report_prot_num`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Accident_report_Vehicles1`
    FOREIGN KEY (`Vehicles_code`)
    REFERENCES `mydb`.`Vehicles` (`Vehicles_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Doctor`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Doctor` (
  `Doctor_id` INT NOT NULL,
  `Doctor_license_number` INT NOT NULL,
  `Doctor_specialty` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Doctor_id`),
  UNIQUE INDEX `Doctor_id_UNIQUE` (`Doctor_id` ASC),
  UNIQUE INDEX `Doctor_license_number_UNIQUE` (`Doctor_license_number` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Medical_activity`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Medical_activity` (
  `Medical_activity_code` INT NOT NULL,
  `Medical_activity_description` VARCHAR(45) NOT NULL,
  `Medical_activity_appliance` VARCHAR(45) NOT NULL,
  `Doctor_id` INT NOT NULL,
  PRIMARY KEY (`Medical_activity_code`, `Doctor_id`),
  UNIQUE INDEX `Medical_activity_code_UNIQUE` (`Medical_activity_code` ASC),
  INDEX `fk_Medical_activity_Doctor1_idx` (`Doctor_id` ASC),
  CONSTRAINT `fk_Medical_activity_Doctor1`
    FOREIGN KEY (`Doctor_id`)
    REFERENCES `mydb`.`Doctor` (`Doctor_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Patient`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Patient` (
  `Patient_hospitalization_code` INT NOT NULL,
  `Patient_hospitalization_date` DATE NOT NULL,
  `Patient_hospitalization_hour` TIME(60) NOT NULL,
  `Patient_release_date` DATE NOT NULL,
  `Patient_release_hour` TIME(60) NOT NULL,
  `Patient_seriousness` VARCHAR(45) NOT NULL,
  `Patient_damage_description` TEXT(60) NOT NULL,
  `Patient_medical_activity_duration` TIME(60) NOT NULL,
  `Patient_medical_activity_result` VARCHAR(45) NOT NULL,
  `Person_code` INT NOT NULL,
  `Person_Identity_card_code` INT NOT NULL,
  PRIMARY KEY (`Patient_hospitalization_code`, `Person_code`, `Person_Identity_card_code`),
  UNIQUE INDEX `Patient_import_code_UNIQUE` (`Patient_hospitalization_code` ASC),
  INDEX `fk_Patient_import_Person1_idx` (`Person_code` ASC, `Person_Identity_card_code` ASC),
  CONSTRAINT `fk_Patient_import_Person1`
    FOREIGN KEY (`Person_code` , `Person_Identity_card_code`)
    REFERENCES `mydb`.`Person` (`Person_code` , `Identity_card_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Alcotest`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Alcotest` (
  `Alcotest_type` INT NOT NULL,
  `Alcotest_hour` TIME(60) NOT NULL,
  `Alcotest_result` VARCHAR(45) NOT NULL,
  `Person_code` INT NOT NULL,
  `Person_Identity_card_code` INT NOT NULL,
  PRIMARY KEY (`Person_code`, `Person_Identity_card_code`),
  CONSTRAINT `fk_Alcotest_Person1`
    FOREIGN KEY (`Person_code` , `Person_Identity_card_code`)
    REFERENCES `mydb`.`Person` (`Person_code` , `Identity_card_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`Patient_has_Medical_activity`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`Patient_has_Medical_activity` (
  `Patient_hospitalization_code` INT NOT NULL,
  `Patient_Person_code` INT NOT NULL,
  `Patient_Person_Identity_card_code` INT NOT NULL,
  `Medical_activity_code` INT NOT NULL,
  `Medical_activity_Doctor_id` INT NOT NULL,
  PRIMARY KEY (`Patient_hospitalization_code`, `Patient_Person_code`, `Patient_Person_Identity_card_code`, `Medical_activity_code`, `Medical_activity_Doctor_id`),
  INDEX `fk_Patient_has_Medical_activity_Medical_activity1_idx` (`Medical_activity_code` ASC, `Medical_activity_Doctor_id` ASC),
  INDEX `fk_Patient_has_Medical_activity_Patient1_idx` (`Patient_hospitalization_code` ASC, `Patient_Person_code` ASC, `Patient_Person_Identity_card_code` ASC),
  CONSTRAINT `fk_Patient_has_Medical_activity_Patient1`
    FOREIGN KEY (`Patient_hospitalization_code` , `Patient_Person_code` , `Patient_Person_Identity_card_code`)
    REFERENCES `mydb`.`Patient` (`Patient_hospitalization_code` , `Person_code` , `Person_Identity_card_code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Patient_has_Medical_activity_Medical_activity1`
    FOREIGN KEY (`Medical_activity_code` , `Medical_activity_Doctor_id`)
    REFERENCES `mydb`.`Medical_activity` (`Medical_activity_code` , `Doctor_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
