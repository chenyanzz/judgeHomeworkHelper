package main

import "C"
import (
  "strconv"
  "net/http"
  "net/url"
  "io/ioutil"
  "io"
  "encoding/json"
  "errors"
  "fmt"
  "net/http/cookiejar"
  "strings"
  "regexp"
  "github.com/PuerkitoBio/goquery"
)

var jar *cookiejar.Jar
var client http.Client

func init() {
  jar, _ = cookiejar.New(nil)
  client = http.Client{
    Jar: jar,
  }
  err := getCookie()
  if err != nil {
    panic(err)
  }
}

func getCookie() (error) {
  r, err := client.Get("http://www.zhixue.com/Login.html")
  if err != nil {
    return err
  }
  defer r.Body.Close()
  return err
}

func Login(username, password string) (err error) {
  r, err := client.PostForm("http://www.zhixue.com/weakPwdLogin/?from=web_login",
  url.Values{"code": {""}, "loginName": {username}, "password": {password}})
  if err != nil {
    return err
  }
  defer r.Body.Close()
  m, err := parse(r.Body)
  if err != nil {
    return err
  }
  if m["result"] != "success" {
    return errors.New("用户名或密码错误")
  }
  rJ, err := client.Get("http://sso.zhixue.com/sso_alpha//Login?service=http://www.zhixue.com:80/ssoservice.jsp&callback=jQuery1910756449938779469_1532499984601")
  if err != nil {
    return err
  }
  defer rJ.Body.Close()
  mJ, err := parseJquery(rJ.Body)
  if err != nil {
    return err
  }
  if mJ["result"] != "success" {
    return errors.New("sso登录失败")
  }
  rJ2_url := fmt.Sprintf("http://sso.zhixue.com/sso_alpha//Login?service=http://www.zhixue.com:80/ssoservice.jsp"+
  "&callback=jQuery1910756449938779469_1532499984601&"+
  "username=%s"+
  "&password=%s"+
  "&sourceappname=tkyh,tkyh&key=id&_eventId=submit"+
  "&lt=%s"+
  "&execution=%s",
  m["data"].(string), password, mJ["data"].(map[string]interface{})["lt"].(string), mJ["data"].(map[string]interface{})["execution"].(string))
  rJ2, err := client.Get(rJ2_url)
  if err != nil {
    return err
  }
  defer rJ2.Body.Close()
  mJ2, err := parseJquery(rJ2.Body)
  if err != nil {
    return err
  }
  if mJ2["result"] != "success" {
    return errors.New("sso登录失败")
  }
  rS, err := client.PostForm("http://www.zhixue.com/ssoservice.jsp",
  url.Values{"action": {"Login"},
  "password": {password},
  "ticket": {mJ2["data"].(map[string]interface{})["st"].(string)},
  "username": {m["data"].(string)},
})
if err != nil {
  return err
}
defer rS.Body.Close()
body, _ := ioutil.ReadAll(rS.Body)
if strings.Index(string(body), "success") < 0 {
  return errors.New("sso登陆失败")
}
r1, err := client.PostForm("http://www.zhixue.com/loginSuccess/",
url.Values{"userId": {m["data"].(string)}})
if err != nil {
  return err
}
defer r1.Body.Close()
m1, err := parse(r1.Body)
if m1["result"] != "success" {
  return errors.New("登录失败")
}
return nil
}

func parse(reader io.Reader) (m map[string]interface{}, err error) {
  body, err := ioutil.ReadAll(reader)
  if err != nil {
    return nil, err
  }
  err = json.Unmarshal(body, &m)
  return
}

func parseJquery(reader io.Reader) (m map[string]interface{}, err error) {
  r, _ := regexp.Compile(".*(\\'.*\\').*")
  body, err := ioutil.ReadAll(reader)
  if err != nil {
    return nil, err
  }
  s := strings.Replace(string(body), "\n", "", -1)
  s = r.FindStringSubmatch(s)[1]
  s = strings.Replace(s, "\\", "", -1)
  s = strings.Replace(s, "'", "", -1)
  err = json.Unmarshal([]byte(s), &m)
  return
}

func GetHomeworkList(t string) (m []map[string]string, err error) {
  r1,_ := regexp.Compile("\"lastPage\":([0-9]),")
  r, err := client.Get(fmt.Sprintf("http://www.zhixue.com/homework/main/sumHomework/main/?hasEnded=%s&pageIndex=1",t))
  if err != nil {
    return nil, err
  }
  defer r.Body.Close()
  doc, err := goquery.NewDocumentFromReader(r.Body)
  if err != nil {
    return nil, err
  }
  doc1,_ := doc.Html()
  s := r1.FindStringSubmatch(doc1)[1]
  index,err := strconv.Atoi(s)
  if err != nil{
    return nil,err
  }
  doc.Find("dd").Each(func(i int, selection *goquery.Selection) {
    m_ := make(map[string]string)
    m_["name"] = selection.Find(".name").Text()
    m_["time"] = selection.Find(".time").Text()
    m_["class"] = selection.Find(".class").Text()
    m_["subject"] = selection.Find(".sec").Text()
    m_["url"], _ = selection.Find(".recordUserLog").Eq(1).Attr("hrefurl")
    m_["sumbit"] = selection.Find(".per").Text()
    m_["homeworkId"], _ = selection.Find(".hw_urge").Attr("data-id")
    m = append(m, m_)
  })
  for i:=2;i<index;i+=1{
    r, err := client.Get(fmt.Sprintf("http://www.zhixue.com/homework/main/sumHomework/main/?hasEnded=%s&pageIndex=%d",t,i))
    if err != nil {
      return nil, err
    }
    defer r.Body.Close()
    doc, err := goquery.NewDocumentFromReader(r.Body)
    if err != nil {
      return nil, err
    }
    doc.Find("dd").Each(func(i int, selection *goquery.Selection) {
      m_ := make(map[string]string)
      m_["name"] = selection.Find(".name").Text()
      m_["time"] = selection.Find(".time").Text()
      m_["class"] = selection.Find(".class").Text()
      m_["subject"] = selection.Find(".sec").Text()
      m_["url"], _ = selection.Find(".recordUserLog").Eq(1).Attr("hrefurl")
      m_["sumbit"] = selection.Find(".per").Text()
      m_["homeworkId"], _ = selection.Find(".hw_urge").Attr("data-id")
      m = append(m, m_)
    })
  }
  return
}

func GetHomeworkFromUrl(url string) (m []map[string]interface{}, err error) {
  r, err := client.Get(url)
  if err != nil {
    return
  }
  defer r.Body.Close()
  body, err := ioutil.ReadAll(r.Body)
  if err != nil {
    return
  }
  reg, _ := regexp.Compile("var topicPackStuStats =(.*)\n.*")
  s := reg.FindStringSubmatch(string(body))[1]
  err = json.Unmarshal([]byte(s[:len(s)-2]), &m)
  return
}

func GetAnswerFromUrl(url string) (m []map[string]interface{},err error) {
  r, err := client.Get(url)
  if err != nil {
    return
  }
  defer r.Body.Close()
  body, err := ioutil.ReadAll(r.Body)
  if err != nil {
    return
  }
  reg,_ := regexp.Compile("var paperInfo =(.*)\n.*")
  s := reg.FindStringSubmatch(string(body))[1]
  err = json.Unmarshal([]byte(s[:len(s)-2]),&m)
  return
}

func SaveCorrectResult(userId, homeworkId, topicPackId, answerDetail string) error {
  r, err := client.PostForm("http://www.zhixue.com/homework/main/correct/saveCorrectResult/",
  url.Values{"userId": {userId}, "homeworkId": {homeworkId}, "topicPackId": {topicPackId}, "answerDetail": {answerDetail}})
  if err != nil {
    return err
  }
  defer r.Body.Close()
  body, err := ioutil.ReadAll(r.Body)
  if string(body) != "{\"result\":\"success\"}" {
    return errors.New("提交错误！")
  }
  return nil
}


//export C_Login
func C_Login(username,password *C.char) *C.char {
  err := Login(C.GoString(username),C.GoString(password))
  if err != nil{
    return C.CString(err.Error())
  }
  return C.CString("登录成功！")
}

//export C_GetHomeworkList
func C_GetHomeworkList(isEnd *C.char) *C.char{
  m,err := GetHomeworkList(C.GoString(isEnd))
  if err != nil{
    return C.CString(err.Error())
  }
  b,err := json.Marshal(m)
  if err!=nil{
    return C.CString(err.Error())
  }
  return C.CString(string(b))
}

//export C_GetHomeworkFromUrl
func C_GetHomeworkFromUrl(url *C.char)(c1 *C.char){
  m,err:= GetHomeworkFromUrl(C.GoString(url))
  if err != nil{
    return C.CString(err.Error())
  }
  b,err := json.Marshal(m)
  if err!= nil{
    return C.CString(err.Error())
  }
  return C.CString(string(b))
}

//export C_GetAnswerFromUrl
func C_GetAnswerFromUrl(url *C.char)(c1 *C.char){
  m,err:= GetAnswerFromUrl(C.GoString(url))
  if err != nil{
    return C.CString(err.Error())
  }
  b,err := json.Marshal(m)
  if err!= nil{
    return C.CString(err.Error())
  }
  return C.CString(string(b))
}

//export C_SaveCorrectResult
func C_SaveCorrectResult(userId, homeworkId, topicPackId, answerDetail *C.char) *C.char {
  err := SaveCorrectResult(C.GoString(userId),C.GoString(homeworkId),C.GoString(topicPackId),C.GoString(answerDetail))
  if err!= nil{
    return C.CString(err.Error())
  }
  return C.CString("提交成功！")
}

func main()  {

}
