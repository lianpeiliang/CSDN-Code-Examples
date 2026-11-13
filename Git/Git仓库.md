# Git仓库名称区别

## Prompt:
Git 中，存放在远程服务器上的原始代码仓库，以及通过克隆操作下载到自己电脑上的代码仓库，分别叫什么？

## Response:
在 Git 中，通常分为 **远程仓库（Remote Repository）** 和 **本地仓库（Local Repository）**。

## 术语备注

- **Prompt**：提问或提示词，在本文中指提出的问题。
- **Response**：回复或回答，在本文中指对问题的解答。

| 类型 | 英文名称 | 说明 |
|--------|--------|--------|
| 远程仓库 | Remote Repository | 托管在 GitHub、GitLab 或服务器上的仓库 |
| 本地仓库 | Local Repository | 通过 `git clone` 克隆到自己电脑上的仓库 |
| 工作区 | Working Directory / Working Tree | 本地仓库中实际编辑代码文件的地方 |

例如：

```bash
git clone https://github.com/user/project.git
```

克隆完成后：

- **GitHub 上的仓库**：远程仓库
- **电脑上的 project 目录（包含 `.git`）**：本地仓库
- **project 目录下的代码文件**：工作区

### 它们之间的关系

**远程仓库（Remote）**

GitHub / GitLab / Git 服务器

git pull / fetch   git push

**本地仓库（Local）**

包含 .git 的项目目录

---

**工作区（Working Tree）**

实际编写、修改代码

---

另外，你可能会经常遇到一个名称：**`origin`**。

`origin` 是 `git clone` 默认给源远程仓库设置的名称。

可以通过以下命令查看：

```bash
git remote -v
```

