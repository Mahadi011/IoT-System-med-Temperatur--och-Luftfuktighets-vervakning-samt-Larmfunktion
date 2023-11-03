

import mysql.connector


#Creating a class for handel database
class DataHandler:
    #Constructor for creat database
    def __init__(self):
        #Database credentials
        self.conn = mysql.connector.connect(
            host='host name',
            user='DB user',
            password='DB password',
            database='tempareture')
        self.my_cursor = self.conn.cursor()

    #Method to creat a table in database
    def create_table(self):
        create_table_query = "CREATE TABLE TempData(Id int PRIMARY KEY AUTO_INCREMENT, Tempc VARCHAR(10), humidity VARCHAR(10))"
        self.my_cursor.execute(create_table_query)

    #Method to insert data in the table
    def insert_data(self, tempC, humidity):
        insert_data_query = "INSERT INTO TempData(Tempc, humidity) VALUES (%s, %s)"
        data = (tempC, humidity)
        self.my_cursor.execute(insert_data_query, data)
        self.conn.commit()

    #Method to close connection
    def close_connection(self):
        self.my_cursor.close()
        self.conn.close()

    #Method to delete the table
    def drop_table(self):
        drop_table_quarry="DROP TABLE TempData "
        self.my_cursor.execute(drop_table_quarry)