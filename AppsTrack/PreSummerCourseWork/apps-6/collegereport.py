import smtplib, os, click
from bs4 import BeautifulSoup
import MySQLdb

def convertResultIntoTable(result, soup, table):
    for row in result:
        tr = soup.new_tag('tr')
        table.append(tr)
        for tdString in row:
            td = soup.new_tag('td', align='center')
            tr.append(td)
            td.string = str(tdString)

@click.command()
@click.option('-h', '--host', default='127.0.0.1', help='mysql host')
@click.option('-u', '--username', default='root', help='mysql username')
@click.option('-P', '--password',  prompt=True, hide_input=True, confirmation_prompt=True, help='Password for db user')
@click.option('-d', '--db-name', help='Database name', default='mrnd')
@click.option('-e', '--to-email', help='Specify email', default='rishabjainchintu29@gmail.com')
def sendreports(to_email, username, password, host, db_name):
    db_connection = MySQLdb.connect(host, username, password, db_name)
    server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
    server.ehlo()

    login = os.environ['EMAIL']
    password = os.environ['PASSWORD']
    server.login(login, password)

    message = '<table></table>'
    soup = BeautifulSoup(message)

    # Insert column headings
    tr = soup.new_tag('tr')
    name = soup.new_tag('th')
    name.string = 'Name'
    tr.append(name)
    college = soup.new_tag('th')
    college.string = 'College'
    tr.append(college)
    transform = soup.new_tag('th')
    transform.string = 'transform'
    tr.append(transform)
    from_custom_base26 = soup.new_tag('th')
    from_custom_base26.string = 'from_custom_base26'
    tr.append(from_custom_base26)
    get_pig_latin = soup.new_tag('th')
    get_pig_latin.string = 'get_pig_latin'
    tr.append(get_pig_latin)
    top_chars = soup.new_tag('th')
    top_chars.string = 'top_chars'
    tr.append(top_chars)
    total = soup.new_tag('th')
    total.string = 'total'
    tr.append(total)
    table = soup.table
    table.append(tr)

    # Fetch data
    cursor = db_connection.cursor()
    cursor.execute('SELECT S.name, S.college, M.transform, M.from_custom_base26, M.get_pig_latin, M.top_chars, M.total FROM students S, marks M WHERE M.dbname = S.dbname ORDER BY S.college')
    result = cursor.fetchall()

    # Insert data into table
    convertResultIntoTable(result, soup, table)
    cursor.close()

    soup.append(soup.new_tag('br'))

    # Insert Heading tags
    soup.append(soup.new_tag('table'))
    table = tuple(soup.find_all('table'))[1]
    college = soup.new_tag('th')
    college.string = 'College'
    minHeading = soup.new_tag('th')
    minHeading.string = 'Min'
    maxHeading = soup.new_tag('th')
    maxHeading.string = 'Max'
    avg_heading = soup.new_tag('th')
    avg_heading.string = 'Avg'
    count_heading = soup.new_tag('th')
    count_heading.string = 'Count'

    tr = soup.new_tag('tr')
    tr.append(college)
    tr.append(minHeading)
    tr.append(maxHeading)
    tr.append(avg_heading)
    tr.append(count_heading)
    table.append(tr)

    # Fetch data
    cursor = db_connection.cursor()
    cursor.execute('SELECT S.college, MIN(M.total), MAX(M.total), AVG(M.total), COUNT(M.total) FROM marks M, students S WHERE M.dbname = S.dbname GROUP BY S.college ORDER BY S.college')
    result = cursor.fetchall()

    # Put data into table
    convertResultIntoTable(result, soup, table)
    cursor.close()

    message = '''From: {} Rishabh Jain
To: {}
Subject: {}
Content-Type: text/html

{}
'''.format(login, to_email, 'Reports', str(soup))

    server.sendmail(to_email, login, message)

if __name__ == '__main__':
    sendreports()

