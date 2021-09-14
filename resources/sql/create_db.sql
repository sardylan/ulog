CREATE TABLE IF NOT EXISTS qso
(
    id              INTEGER   NOT NULL
        CONSTRAINT qso_id_uniq
            UNIQUE
        CONSTRAINT qso_pk
            PRIMARY KEY AUTOINCREMENT,
    dt_start        TIMESTAMP NOT NULL,
    dt_end          TIMESTAMP,
    remote_callsign TEXT,
    remote_op_name  TEXT,
    tx_rst          TEXT,
    rx_rst          TEXT,
    tx_frequency    NUMERIC,
    rx_frequency    NUMERIC,
    modulation      TEXT,
    qth             TEXT,
    locator         TEXT
);

CREATE INDEX IF NOT EXISTS qso_dt_start_idx ON qso (dt_start);
CREATE INDEX IF NOT EXISTS qso_remote_callsign_idx ON qso (remote_callsign);
CREATE INDEX IF NOT EXISTS qso_modulation_idx ON qso (modulation);

CREATE TABLE IF NOT EXISTS fields
(
    id          INTEGER NOT NULL
        CONSTRAINT fields_id_uniq
            UNIQUE
        CONSTRAINT fields_pk
            PRIMARY KEY AUTOINCREMENT,
    table_name  TEXT    NOT NULL,
    field_name  TEXT    NOT NULL,
    field_type  TEXT    NOT NULL,
    position    INTEGER NOT NULL DEFAULT 0,
    description TEXT    NOT NULL,
    enabled     BOOLEAN NOT NULL DEFAULT TRUE
);

CREATE INDEX IF NOT EXISTS fields_table_name_idx ON fields (table_name);
CREATE INDEX IF NOT EXISTS fields_table_name_field_name_idx ON fields (table_name, field_name);

INSERT INTO fields (table_name, field_name, field_type, position, description)
VALUES ('qso', 'dt_start', 'QDateTime', -1, 'Start'),
       ('qso', 'remote_callsign', 'QString', 0, 'Callsign'),
       ('qso', 'tx_rst', 'QString', 1, 'Sent RST'),
       ('qso', 'rx_rst', 'QString', 2, 'Recv RST'),
       ('qso', 'remote_op_name', 'QString', 3, 'OP Name'),
       ('qso', 'qth', 'QString', 4, 'QTH');
