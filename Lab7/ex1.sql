SET SERVEROUTPUT ON;

CREATE OR REPLACE TRIGGER display_driver_info_on_participation
AFTER INSERT ON PARTICIPATED
FOR EACH ROW
DECLARE
    v_driver_name PERSON.name%TYPE;
    v_driver_address PERSON.address%TYPE;
BEGIN
    -- Fetch the driver's name and address from the PERSON table based on the driver_id from PARTICIPATED table
    SELECT name, address
    INTO v_driver_name, v_driver_address
    FROM PERSON
    WHERE driver_id = :NEW.driver_id;

    -- Display driver information using DBMS_OUTPUT
    DBMS_OUTPUT.PUT_LINE('Driver ID: ' || :NEW.driver_id);
    DBMS_OUTPUT.PUT_LINE('Name: ' || v_driver_name);
    DBMS_OUTPUT.PUT_LINE('Address: ' || v_driver_address);
    DBMS_OUTPUT.PUT_LINE('Car Registration Number: ' || :NEW.regno);
    DBMS_OUTPUT.PUT_LINE('Accident Report Number: ' || :NEW.report_no);
    DBMS_OUTPUT.PUT_LINE('Damage Amount: ' || :NEW.damage_amount);

END;
/
