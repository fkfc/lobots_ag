SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `lobots_ag` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
USE `lobots_ag` ;

-- -----------------------------------------------------
-- Table `lobots_ag`.`batch`
-- Agrupa as populações compatíveis
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `lobots_ag`.`batch` (
  `idbatch` INT NOT NULL AUTO_INCREMENT,
  `datahora` DATETIME NULL,
  `tam_pop` INT NULL,
  PRIMARY KEY (`idbatch`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `lobots_ag`.`geracao`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `lobots_ag`.`geracao` (
  `idgeracao` INT NOT NULL AUTO_INCREMENT,
  `batch` INT NOT NULL,
  `datahora` DATETIME NULL,
  PRIMARY KEY (`idgeracao`),
  INDEX `fk_table1_batch_idx` (`batch` ASC),
  CONSTRAINT `fk_table1_batch`
    FOREIGN KEY (`batch`)
    REFERENCES `lobots_ag`.`batch` (`idbatch`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `lobots_ag`.`individuo`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `lobots_ag`.`individuo` (
  `idindividuo` INT NOT NULL AUTO_INCREMENT,
  `fitness` DOUBLE NULL,
  `cromossomo` BLOB NULL,
  `geracao` INT NOT NULL,
  PRIMARY KEY (`idindividuo`),
  INDEX `fk_individuo_geracao1_idx` (`geracao` ASC),
  CONSTRAINT `fk_individuo_geracao1`
    FOREIGN KEY (`geracao`)
    REFERENCES `lobots_ag`.`geracao` (`idgeracao`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
