-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema cars2019DB
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema cars2019DB
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `cars2019DB` ;
USE `cars2019DB` ;

-- -----------------------------------------------------
-- Table `cars2019DB`.`dataTable`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `cars2019DB`.`dataTable` (
  `timestep` DOUBLE UNSIGNED NOT NULL,
  `device_id` INT NOT NULL,
  `real_lat` DOUBLE UNSIGNED NOT NULL,
  `real_long` DOUBLE UNSIGNED NOT NULL,
  `predicted_lat` DOUBLE UNSIGNED NOT NULL,
  `predicted_long` DOUBLE UNSIGNED NOT NULL,
  `real_RSSI` DOUBLE UNSIGNED NOT NULL,
  `real_throughput` DOUBLE UNSIGNED NOT NULL,
  `predicted_RSSI` DOUBLE UNSIGNED NOT NULL,
  `predicted_throughput` DOUBLE UNSIGNED NOT NULL,
  PRIMARY KEY (`device_id`),
  UNIQUE INDEX `device_id_UNIQUE` (`device_id` ASC))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
